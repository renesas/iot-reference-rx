import os
from pathlib import Path
import subprocess

curr_path = Path(__file__).parent
top_path = curr_path.parents[1]

prev_path = os.getcwd()
os.chdir(top_path)
cmd = ['git', 'pull', '--progress', '-v', '--no-rebase', 'origin']
print(' '.join(cmd))
subprocess.run(cmd)
cmd = ['git', 'submodule', 'sync', '--recursive']
print(' '.join(cmd))
subprocess.run(cmd)
cmd = ['git', 'submodule', 'update', '--init', '--recursive']
print(' '.join(cmd))
subprocess.run(cmd)
os.chdir(prev_path)
