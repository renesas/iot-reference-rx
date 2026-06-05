import json
from pathlib import Path
import sys

def read_json(filepath, isExit=True):
    try:
        with open(Path(filepath)) as file_json:
            json_obj = json.load(file_json)
    except FileNotFoundError as err:
        print(err)
        if isExit:
            sys.exit(1)
        else:
            json_obj = {}
    except json.decoder.JSONDecodeError as err:
        print('JSON Decode Error has occurred.')
        print(err)
        if isExit:
            sys.exit(1)
        else:
            json_obj = {}
    return json_obj
