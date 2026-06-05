from pathlib import Path
import re
import sys

sys.path.append(str(Path(__file__).parents[2]))
from misc.filter_comment import filter_comment
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
        sc_option = read_json(top_path / project['ConfigCheck'])

        print(f'### Checking {proj_name} ###')
        if proj_name.endswith('_gcc'):
            proj_name = proj_name[:-4]
        sc_file_path = (workspace_path / proj_path / proj_name).with_suffix('.scfg')
        r_config_path = workspace_path / proj_path / 'src/smc_gen/r_config'
        frtos_config_path = workspace_path / proj_path / 'src/frtos_config/FreeRTOSConfig.h'

        # Open ".scfg" file as xml
        _, root = read_xml(sc_file_path, False)
        if root is None:
            print(f"Checking {proj_name} skipped.")
            continue

        # Extract all "configuration" elements
        configurations = root.findall('.//configuration')

        grid_item_dict = {}
        for configuration in configurations:
            name = configuration.get('name')
            component = configuration.find('component')
            if component is not None:
                grid_items = component.findall('.//gridItem')
                for grid_item in grid_items:
                    grid_value = clean_value(grid_item.get('selectedIndex'))
                    option_tbl = sc_option.get(grid_item.get('id'))
                    if option_tbl is not None:
                        try:
                            # Try integer index (for list)
                            grid_value = option_tbl[int(grid_value)]
                        except (ValueError, KeyError, IndexError, TypeError):
                            # Fallback: try as string key (for dict)
                            grid_value = option_tbl.get(grid_value, grid_value)
                    grid_item_dict[name, grid_item.get('id')] = conv_value(grid_value)

        # Find ".h" files in "r_config" folder
        conf_lines = []
        for file_path in r_config_path.glob('*.h'):
            with open(file_path, 'r') as file:
                lines = file.read()
            conf_lines += filter_comment(lines).splitlines()

        if frtos_config_path.is_file():
            with open(frtos_config_path, 'r') as file:
                frtos_lines = file.read()
            conf_lines += filter_comment(frtos_lines).splitlines()

        # Find #define macros
        define_macros = [line.split(None, 2) for line in conf_lines if line.lstrip().startswith('#define')]
        # Remove space and parentheses
        defines = {tokens[1]: conv_value(clean_value(tokens[2])) for tokens in define_macros if len(tokens) == 3}

        error_count = 0
        for key, value in grid_item_dict.items():
            name, grid_id = key
            if grid_id in defines:
                if defines[grid_id] != grid_item_dict[key]:
                    print(f'Error: {grid_id} in {name} does not match macro value. XML: {value}, Header: {defines[grid_id]}')
                    error_count += 1

        if error_count > 0:
            isError = True
            print(f'Number of error messages: {error_count}')
        else:
            print('No error or warning found.')

    if isError:
        print('The configurations should be reviewed due to configuration mismatch.')
        sys.exit(1)

def clean_value(text):
    if issubclass(type(text), str):
        # Remove space characters
        text = ''.join(text.split())
        # Remove parentheses
        return text.replace('(', '').replace(')', '')
    else:
        return text

def conv_value(text):
    string = str(text)
    hex_match = re.match(r'0[xX][0-9a-fA-F]+', string)
    octal_match = re.match(r'0[0-7]+', string)
    if hex_match:
        return int(hex_match.group(0), 16)
    elif octal_match:
        return int(octal_match.group(0), 8)
    else:
        try:
            return int(string)
        except ValueError:
            return string

if __name__ == '__main__':
    main()
