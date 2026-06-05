import os
import sys

def read_environ(name, isExit=True):
    try:
        value = os.environ[name]
    except KeyError as err:
        print(f'KeyError: {err} not found')
        if isExit:
            sys.exit(1)
        else:
            value = ''
    return value

def set_environ(name, value):
    os.environ[name] = str(value)
