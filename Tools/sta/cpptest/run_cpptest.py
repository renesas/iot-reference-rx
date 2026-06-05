import os
from pathlib import Path
import shutil
import subprocess
import sys

sys.path.append(str(Path(__file__).parents[2]))
from build.e2studio.exec_build import exec_build
from misc.util_environ import read_environ, set_environ
from misc.util_json import read_json

def main():
    curr_path = Path(__file__).parent
    top_path = curr_path.parents[2]

    environ = read_json(top_path / read_environ('CONFIG_CI_ENVIRON_PATH'))

    workspace_path = top_path / Path(environ['AfrPath'])
    proj_listname = environ['ProjectListPath']
    proj_list = read_json(proj_listname)

    tag_name = read_environ('CI_RUNNER_DESCRIPTION')
    sta_environs = read_json(curr_path / 'environ.json')
    try:
        sta_environ = sta_environs[tag_name]
    except KeyError:
        print(f"Add a configuration to run cpptest to '{curr_path / 'environ.json'}'.")
        sys.exit(1)

    cpptest_path = Path(sta_environ['CpptestPath'])
    cpptest_bin_path = cpptest_path / 'bin'
    rx_toolchain_path = Path(sta_environ['RxToolChainPath'])
    make_path = Path(sta_environ['MakePath']) / 'Utilities'
    jre_path = cpptest_path / 'bin/jre/bin'
    eclipse_path = Path(sta_environ['EclipsePath'])
    os.environ['PATH'] = '{};{};{};{};{};{}'.format(cpptest_path, cpptest_bin_path, rx_toolchain_path,\
                                                    make_path, jre_path, os.getenv('PATH'))

    # Remove workspace files
    e2s_metadata_path = top_path / '.metadata'
    cpptest_metadata_path = top_path / '.cpptest'
    shutil.rmtree(e2s_metadata_path, ignore_errors=True)
    shutil.rmtree(cpptest_metadata_path, ignore_errors=True)

    for proj in (proj for proj in proj_list if proj.get('StaConf') if proj.get('StaConf').get('CpptestConf')):
        proj_name = proj['ProjectName']
        proj_conf = proj['StaConf']['CpptestConf']
        proj_path = workspace_path / proj['ProjectPath']

        log_path = top_path / proj['LogPath'] / 'cpptest'
        log_path.mkdir(parents=True, exist_ok=True)

        print(f'### Analyzing {proj_name} ###')
        log_proj_path = log_path / proj_name
        report_path = log_proj_path / 'report'

        # Create and clean report folder
        report_path.mkdir(parents=True, exist_ok=True)
        for file in (fd for fd in log_proj_path.iterdir() if fd.is_file()):
            file.unlink()
        for file in (fd for fd in report_path.iterdir() if fd.is_file()):
            file.unlink()

        # Build
        print('### Building {} ###'.format(proj_name))
        proj_file_path = proj_path / '.cproject'
        build_conf_path = proj_path / 'HardwareDebug'
        if not proj_file_path.is_file():
            print(f"Building {proj_name} skipped.")
            continue
        shutil.rmtree(build_conf_path, ignore_errors=True)
        _, output = exec_build(proj_path, proj_name)
        with open(log_proj_path / 'log_sta_build.txt', 'w', encoding='utf-8') as file:
            file.writelines(output)

        # Create BDF
        print('### Create BDF ###')

        bdf_file_path = proj_path / 'cpptestscan.bdf'
        os.environ['CPPTEST_SCAN_OUTPUT_FILE'] = str(bdf_file_path)
        os.environ['CPPTEST_SCAN_PROJECT_NAME'] = proj_name
        bdf_file_path.unlink(missing_ok=True)

        if not (build_conf_path := proj_path / 'HardwareDebug').is_dir():
            print(f'"HardwareDebug" folder not found.')
            continue
        prev_path = os.getcwd()
        os.chdir(build_conf_path)
        cmd = ['make.exe', '-j8', 'clean']
        cmd2 = ['cpptesttrace', '--cpptesttraceResponseFilePrefix=-subcommand=', 'make.exe', '-j8', 'all']
        print(' '.join(cmd))
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        stdout,_ = proc.communicate()
        print(' '.join(cmd2))
        proc = subprocess.Popen(cmd2, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        stdout2,_ = proc.communicate()
        with open(log_proj_path / 'log_sta_createbdf.txt', 'w', encoding='utf-8') as file:
            file.writelines(stdout + stdout2)
        os.chdir(prev_path)

        # Runs static analysis
        print('### Runs static analysis ###')

        cmd = ['cpptestcli', '-data', '.', '-import', proj_path, '-resource', proj_name]
        cmd.extend(add_option('-config', top_path / proj_conf['ConfigPath']))
        cmd.extend(add_option('-include', top_path / proj_conf['IncludePath']))
        cmd.extend(add_option('-exclude', top_path / proj_conf['ExcludePath']))
        cmd.extend(add_option('-bdf', bdf_file_path))
        set_path = [top_path / p for p in proj_conf['SettingPath']]
        if not (config_path := top_path / proj_conf['CompilerPath']).is_file():
            print(f'The setting file {config_path.name} not found.')
            continue
        add_mod_environ()
        subprocess.run(['git', 'checkout', '--quiet', config_path])
        set_path.append(create_compiler_file(config_path, proj_name))
        cmd.extend(add_option('-settings', set_path))
        cmd.extend(['-report', report_path, '-path-to-eclipse', eclipse_path, '-showdetails', '-appconsole', 'stdout'])
        print(' '.join([str(n) for n in cmd]))
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        stdout,_ = proc.communicate()
        with open(log_proj_path / 'log_sta_cpptestcli.txt', 'w', encoding='cp932') as file:
            file.writelines(stdout)

def create_compiler_file(file_path, proj_name):
    with open(file_path, 'r', newline='') as file:
        settings = file.read().replace('${ProjectName}', proj_name)
    with open(file_path, 'w', newline='') as file:
        file.writelines(settings)
    return file_path

def add_mod_environ():
    if (branch_name := read_environ('CI_COMMIT_BRANCH', False)):
        set_environ('CI_COMMIT_BRANCH_BUILD_ID', branch_name.replace('/', '-'))
    return branch_name

def add_option(option, params):
    if params and params is not None:
        if issubclass(type(params), list):
            cmd = []
            for param in params:
                cmd.extend([option, param])
            return cmd
        else:
            return [option, params]

if __name__ == '__main__':
    main()
