# CLI file for `wcics inspect`
# inspect is a python module, so `inspection`

from .run_setup import setup
import os

def main(args):
  res = setup(args)
  
  if res is not None:
    return res
  
  BRANCH = os.environ['WCICS_BRANCH']
  
  return os.system("python3 -i ~/workspace/{branch}/wcics_cli/inspect_defaults.py".format(branch = BRANCH))