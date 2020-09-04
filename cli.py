import os, sys

import wcics_cli.indexing

ports = {
  "keenan" : 4000,
  "hyper-neutrino" : 5000
}

def set_preset(x):
  os.environ['CSCENTER_PRESET'] = x

def main(args):
  if args[0] == 'debug':
    set_preset('debug')

    port = ports.get(os.getlogin(), None)

    if port is None: port = args[1]

    return os.system(f"python3 main.py {port}")

  elif args[0] in ['make', 'clean']:
    return wcics_cli.indexing.main(args)

  elif args[0] == 'test':
    set_preset('testing')

    return os.system("pytest --capture=no wcics/tests -vv")

  elif args[0] == 'prod':
    set_preset("production")

    return os.system("uwsgi --ini csc.ini")

  elif args[0] == 'prod-test':
    set_preset("production")

    return os.system("uwsgi --ini csc-test.ini")

  elif args[0] == 'inspect':
    set_preset('debug')

    return os.system("python3 -i wcics_cli/inspect_defaults.py")

  raise ValueError("No valid command was specified!")

if "CSCENTER_KEYFILE" not in os.environ:
  os.environ['CSCENTER_KEYFILE'] = "/srv/cscenter/keys.json"

sys.exit(main(sys.argv[1:]))
