#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import pathlib
import subprocess
import difflib
import sys
from pathlib import Path
import json
import shutil

# Add parent folder to sys.path to find misc.logger
sys.path.append(str(Path(__file__).parents[2]))
from misc.util_json import read_json
from misc.logger import get_logger, get_file_handler

# ----------------------------
# Windows-safe stdout
# ----------------------------
if os.name == "nt":
    try:
        sys.stdout.reconfigure(encoding="utf-8")
        sys.stderr.reconfigure(encoding="utf-8")
    except AttributeError:
        pass

# ----------------------------
# Initialize logger
# ----------------------------
log_dir = Path("logs")
file_handler = get_file_handler(log_dir)
logger = get_logger(file_handler=file_handler)

# ----------------------------
# Load JSON config
# ----------------------------
def load_config(config_path: Path):
    with open(config_path, "r", encoding="utf-8") as f:
        return json.load(f)

def expand_path(p: str | None) -> Path | None:
    if not p:
        return None
    return Path(os.path.expandvars(p)).expanduser().resolve()

# ----------------------------
# Run clang-format on a file
# ----------------------------
def run_clang_format(file_path: Path, style_file: Path):
    file_path_str = str(file_path.resolve().as_posix())
    style_file_str = str(style_file.resolve().as_posix())

    # Hardcoded clang-format path
    clang_format_exe = r"C:\Program Files\LLVM\bin\clang-format.exe"

    cmd = [clang_format_exe, f"--style=file:{style_file_str}", file_path_str]
    logger.debug(f"Running: {' '.join(cmd)}")
    try:
        proc = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="utf-8",
            check=False
        )
        return proc.returncode, proc.stdout
    except FileNotFoundError:
        logger.error(f"!! Error: clang-format.exe not found at {clang_format_exe}")
        return 1, ""

# ----------------------------
# Check or fix a single file
# ----------------------------
def check_file(file_path: Path, diff_root: Path, base_folder: Path, fix: bool, style_file: Path, packages_path: Path) -> bool:
    original_code = file_path.read_text(encoding="utf-8").replace("\r\n", "\n")
    logger.info(f"[file_path] {file_path}")
    file_path = pathlib.Path(file_path).resolve()
    retcode, formatted_code = run_clang_format(file_path, style_file)

    if retcode != 0:
        logger.error(f"!! run_clang_format failed with return code {retcode} for {file_path}")
        return False

    if original_code != formatted_code:
        if fix:
            file_path.write_text(formatted_code, encoding="utf-8")
            logger.info(f"[FIXED] {file_path}")
            return True
        else:
            # Normalize line endings for diff
            original_lines = original_code.splitlines(keepends=True)
            formatted_lines = formatted_code.splitlines(keepends=True)

            diff = difflib.unified_diff(
                original_lines,
                formatted_lines,
                fromfile=str(file_path),
                tofile=str(file_path) + ".clang_formatted",
                lineterm=""
            )
            diff_text = "".join(diff)

            # Compute relative path from packages_path
            relative_path = file_path.relative_to(packages_path)

            diff_output_path = diff_root / relative_path
            diff_output_path = diff_output_path.with_suffix(diff_output_path.suffix + ".patch")
            diff_output_path.parent.mkdir(parents=True, exist_ok=True)
            diff_output_path.write_text(diff_text, encoding="utf-8")
            logger.debug(f"[DIFF] {diff_output_path}")
            return False
    else:
        logger.debug(f"[OK] {file_path} is OK")
        return True

# ----------------------------
# Recursively check a folder
# ----------------------------
def check_folder_recursive(folder_path: Path, extensions: set[str], diff_root: Path, fix: bool, style_file: Path, packages_path: Path, exclude: set[str]):
    # Convert exclude string paths to resolved Path objects
    exclude_paths = { (folder_path / Path(ex)).resolve() for ex in exclude }

    all_ok = True
    checked = changed = 0
    for file_path in folder_path.rglob("*"):
        if (file_path.is_file() and file_path.suffix in extensions):
             # Exclude if any part of the path contains an ExcludePath keyword
            file_path_str = str(file_path.resolve().as_posix())  # Use as_posix() to normalize path separators
            if any(exclude_keyword in file_path_str for exclude_keyword in exclude):
                continue

            ok = check_file(file_path, diff_root, base_folder=folder_path, fix=fix, style_file=style_file, packages_path=packages_path)
            checked += 1
            if not ok:
                changed += 1
            all_ok = all_ok and ok
    return all_ok, checked, changed

# ----------------------------
# Main
# ----------------------------
def main():
    curr_path = Path(__file__).parent
    top_path = curr_path.parents[2]

    # Load style config instead of project list/config
    clang_config_path = curr_path / 'clang_config.json'
    if not clang_config_path.exists():
        logger.error(f"!! clang_config.json not found: {clang_config_path}")
        sys.exit(1)
    clang_config = read_json(clang_config_path)

    extensions = clang_config.get("file_extensions", [".c", ".h"])
    exclude = set(clang_config.get("ExcludePath", []))
    fix_clang = clang_config.get("fix", False)

    diff_root = top_path / 'diff_output'

    # Resolve .clang-format file
    style_file = curr_path / '.clang-format'
    if not style_file or not style_file.exists():
        logger.error(f"!! .clang-format not found: {style_file}")
        sys.exit(1)

    if fix_clang:
        if diff_root.exists():
            shutil.rmtree(diff_root)
        diff_root.mkdir(parents=True, exist_ok=True)

    overall_ok = True
    total_checked = total_changed = 0

    print(f'### Checking coding style ###')

    folder_path = top_path.resolve()
    if not folder_path.exists() or not folder_path.is_dir():
        logger.error(f"!! Folder not found: {folder_path}")
    else:
        logger.info(f"\n>> Checking folder: {folder_path}")
        ok, checked, changed = check_folder_recursive(
            folder_path, set(extensions), diff_root, fix_clang, style_file, top_path, exclude
        )
        overall_ok = overall_ok and ok
        total_checked += checked
        total_changed += changed
        logger.info(f"--- {folder_path} summary: {checked} checked, {changed} needing changes ---")

    logger.info("\n---- SUMMARY ----")
    logger.info(f"Mode: {'FIX' if fix_clang else 'CHECK'}")
    logger.info(f"Files checked: {total_checked}")
    if not fix_clang:
        logger.info(f"Files needing changes: {total_changed}")

    sys.exit(0 if fix_clang or overall_ok else 1)

if __name__ == "__main__":
    main()

