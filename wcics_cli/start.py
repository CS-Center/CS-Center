# CLI file for `wcics start`

from .run_setup import setup
import os

def main(args):
  res = setup(args)
  
  if res is not None:
    return res
  
  BRANCH = os.environ['WCICS_BRANCH']
    
  if args[0] == 'debug':
    return os.system("python3 ~/workspace/{}/wcics/server/main.py".format(BRANCH))
    
  elif args[0] == 'testing':
    return os.system("pytest --capture=no ~/workspace/{}/wcics/tests -vv".format(BRANCH))