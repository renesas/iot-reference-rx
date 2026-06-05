from pathlib import Path
import sys
import xml.etree.ElementTree as ET

def read_xml(filepath, isExit=True):
    try:
        tree = ET.parse(Path(filepath))
    except (FileNotFoundError, PermissionError, ET.ParseError) as err:
        if isExit:
            print(err)
            sys.exit(1)
        else:
            tree = None; root = None
    else:
        root = tree.getroot()
    return tree, root
