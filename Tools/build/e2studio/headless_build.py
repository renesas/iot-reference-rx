from pathlib import Path
import re
import shutil
import sys

from exec_build import exec_build
sys.path.append(str(Path(__file__).parents[2]))
from check.msg_count.msg_count import msg_count
from misc.logger import get_logger, add_handler, get_file_handler, close_file_handler
from misc.util_environ import read_environ
from misc.util_json import read_json

def main():
    curr_path = Path(__file__).parent
    top_path = curr_path.parents[2]

    environ = read_json(top_path / read_environ('CONFIG_CI_ENVIRON_PATH'))
    proj_list = read_json(top_path / environ['ProjectListPath'])
    ws_path = top_path / environ['AfrPath']
    md_path = top_path / '.metadata'

    logger = get_logger()

    is_build_error = False
    err_proj_names = []

    for proj in proj_list:
        proj_name = proj['ProjectName']
        proj_path = ws_path / proj['ProjectPath']
        proj_file_path = proj_path / '.cproject'
        build_path = proj_path / 'HardwareDebug'

        if not proj_file_path.is_file():
            print(f"Building {proj_name} skipped.")
            continue

        log_path = top_path / proj['LogPath']
        log_path.mkdir(parents=True, exist_ok=True)
        mot_path = top_path / proj['MotPath']
        mot_path.mkdir(parents=True, exist_ok=True)

        shutil.rmtree(md_path, ignore_errors=True)
        shutil.rmtree(build_path, ignore_errors=True)

        log_file_name = 'log_build_' + proj_name + '.txt'
        file_handler = get_file_handler(log_path, log_file_name, 'w', 'utf-8')
        add_handler(logger, file_handler)

        logger.info(f'Building {proj_name}')
        _, output = exec_build(proj_path, proj_name, logger)

        c = re.compile(r'Build (Finished|Failed)\.\s+(\d+)\s+errors')
        for line in reversed(output.splitlines()):
            if (err := c.search(line)):
                if int(err.group(2)) > 0:
                    is_build_error = True
                    err_proj_names.extend([proj_name])
                break
        else:
            print('Warning: build errors have not been checked.')

        logger.debug(output)
        logger.info('Build finished.')

        close_file_handler(logger, file_handler)

        if (is_gcc := 'gcc' in proj_path.name):
            print(f'### "{proj_name}" (gcc) build log summary ###')
        else:
            print(f'### "{proj_name}" build log summary ###')
        if (mc_path := proj.get('MsgCountPath')):
            files = read_json(top_path / mc_path, False)
            msg_count(log_path / log_file_name, files.get(proj_name), is_gcc=is_gcc)
        else:
            msg_count(log_path / log_file_name, is_gcc=is_gcc)

        for file_name in build_path.glob('*.mot'):
            shutil.copy(file_name, mot_path)

    if is_build_error:
        print('Build errors have occurred in the following projects.')
        for n in err_proj_names:
            print(n)
        sys.exit(1)

if __name__ == '__main__':
    main()
