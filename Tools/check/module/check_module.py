from pathlib import Path
import sys

sys.path.append(str(Path(__file__).parents[2]))
from misc.util_environ import read_environ
from misc.util_json import read_json
from misc.util_xml import read_xml

def main():
    curr_path = Path(__file__).parent
    top_path = curr_path.parents[2]

    environ = read_json(top_path / read_environ('CONFIG_CI_ENVIRON_PATH'))
    workspace_path = top_path / environ['AfrPath']
    proj_list_path = top_path / environ['ProjectListPath']
    projects = read_json(proj_list_path)

    isError = False

    for project in projects:
        proj_name = project['ProjectName']
        proj_path = project['ProjectPath']
        expected = read_json(top_path / project['ModuleCheck'])

        print(f'### Checking {proj_name}: the version of the modules used ###')
        if proj_name.endswith('_gcc'):
            proj_name = proj_name[:-4]
        sc_file_path = (workspace_path / proj_path / proj_name).with_suffix('.scfg')

        _, root = read_xml(sc_file_path, False)
        if root is None:
            print(f"Checking {proj_name} skipped.")
            continue
        components = root.findall('.//component')

        label_match = 'Match'; label_mismatch = 'Mismatch'
        label_notfound = 'NotFound'; label_skip = 'Skip'
        labels = (label_match, label_mismatch, label_notfound, label_skip)
        checked = []
        for component in components:
            display = component.get('display')
            version = component.get('version')
            check = {'Display': display, 'Version': version}

            if (exp_version := expected.get(display)) is not None:
                if not exp_version:
                    check['Label'] = label_skip
                elif version == exp_version:
                    check['Label'] = label_match
                else:
                    check['Label'] = label_mismatch
                check['ExpVersion'] = exp_version
            else:
                check['Label'] = label_notfound
            checked.append(check)

        for label in labels:
            print(f'{label}:')
            isNone = True
            for check in checked:
                if check['Label'] == label:
                    isNone = False
                    print(f"  {check['Display']}: {check['Version']}", end='')
                    if check['Label'] == label_mismatch:
                        isError = True
                        print(f", expected: {check['ExpVersion']}", end='')
                    print('')
            if isNone:
                print('  None')

    if isError:
        print('The module versions should be reviewed due to version mismatch.')
        sys.exit(1)

if __name__ == '__main__':
    main()
