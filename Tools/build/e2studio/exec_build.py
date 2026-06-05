import subprocess

def exec_build(proj_path, proj_name, logger=None):
    # Pre-process proj_name to remove _gcc if present
    build_name = proj_name.replace('_gcc', '') if '_gcc' in proj_name else proj_name

    cmd = ['e2studioc.exe', '--launcher.suppressErrors', '-nl', 'en_US', '-nosplash', '-debug', '-consolelog',\
        '-application', 'org.eclipse.cdt.managedbuilder.core.headlessbuild', '-data', '.', '-import', proj_path,\
        '-cleanBuild', build_name]
    if logger:
        logger.info(' '.join([str(n) for n in cmd]))
    else:
        print(' '.join([str(n) for n in cmd]))
    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    return proc.returncode, proc.stdout
