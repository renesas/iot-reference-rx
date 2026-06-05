import re

pats_ccrx = {
    'Error': {
        'Internal error': [r'\):C0'],
        'Error': [r'\):E0'],
        'Fatal error': [r'\):F0']
    },
    'Warning': {
        'Warning': [r'\):W0']
    },
    'Link': {
        'Linker message': [r'^C0', r'^E0', r'^F0', r'^W0']
    }
}
pats_gcc = {
    'Error': {
        'Error': [r'error:']
    },
    'Warning': {
        'Warning': [r'warning:']
    }
}
pat_path = {
    'Error': r'\.\.[\|/].*?\.[c|h]',
    'Warning': r'[A-Z]:[\|/].*?\.[c|h]'
}

def msg_search(msgs, pat):
    c = re.compile(pat)
    return [msg for msg in msgs if c.search(msg)]

def msg_disp(msgs, dict_pats):
    msg_cats = {}
    for n, (cat, pats) in enumerate(dict_pats.items()):
        msg_cats[cat] = []
        for pat in pats:
            msg_cats[cat] += msg_search(msgs, pat)
        if n < len(dict_pats)-1:
            print(f'{cat}: {len(msg_cats[cat])}', end=', ')
        else:
            print(f'{cat}: {len(msg_cats[cat])}')
    for cat in dict_pats.keys():
        if msg_cats[cat]:
            print(f'{cat}:')
            for msg in msg_cats[cat]:
                print(msg)

def msg_extract(msgs, pat):
    c = re.compile(pat)
    msg_ext = []
    for msg in msgs:
        if (det := c.search(msg)):
            msg_ext += [det.group(0)]
    return msg_ext

def msg_disp_file(msgs, dict_pats, pat_path):
    msg_cats = {}
    for cat, pats in dict_pats.items():
        msg_cats[cat] = []
        for pat in pats:
            msg_cats[cat] += msg_search(msgs, pat)
        paths = msg_extract(msg_cats[cat], pat_path)
        msg_cnt = {}
        for path in paths:
            if issubclass(type(msg_cnt.get(path)), int):
                msg_cnt[path] += 1
            else:
                msg_cnt[path] = int(1)
        if msg_cnt:
            print(f'Number of {cat} messages for each file:')
            for path, cnt in sorted(msg_cnt.items()):
                print(f'{path}: {cnt}')

def msg_count(log_path, files=None, is_gcc=False):

    with open(log_path, 'r', newline='') as file:
        lines = [line for line in file.read().splitlines()]

    if is_gcc:
        dict_pats = pats_gcc
    else:
        dict_pats = pats_ccrx

    if files:
        for file in files:
            print(f'Messages for {file}:')
            msg_file = msg_search(lines, file)
            msg_disp(msg_file, dict_pats['Error'])
            msg_disp(msg_file, dict_pats['Warning'])
        msg_disp(lines, dict_pats['Link'])
        msg_disp_file(lines, dict_pats['Error'], pat_path['Error'])
        msg_disp_file(lines, dict_pats['Warning'], pat_path['Warning'])
    else:
        msg_disp(lines, dict_pats['Error'])
        msg_disp_file(lines, dict_pats['Error'], pat_path['Error'])
