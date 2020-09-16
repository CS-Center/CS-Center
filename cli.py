import os, sys

import wcics_cli.indexing

ports = {
  "keenan" : 4000,
  "hyper-neutrino" : 5000
}

DOMAINS = {
  4000 : 'riolku.cscenter.ca',
  5000 : 'neutrino.cscenter.ca'
}

def set_env(k, v):
  os.environ[k] = str(v)

def set_preset(x):
  set_env("CSCENTER_PRESET", x)

def main(args):
  if args[0] == 'prod':
    set_env("CSCENTER_DOMAIN", "cscenter.ca")
    set_preset("production")

    return os.system("uwsgi --ini csc.ini")

  elif args[0] == 'prod-test':
    set_env("CSCENTER_DOMAIN", "prod-test.cscenter.ca")
    set_preset("production")

    return os.system("uwsgi --ini csc-test.ini")

  elif args[0] == 'inspect':
    set_preset('debug')

    return os.system(f"python3 -i inspect_defaults.py {port}")

  port = ports.get(os.getlogin(), None)
  if port is None: port = args[1]

  set_env("CSCENTER_PORT", port)

  DOMAIN = DOMAINS[port]

  if 'local' in sys.argv:
    DOMAIN = f'localhost:{port}'

  set_env("CSCENTER_DOMAIN", DOMAIN)

  if args[0] == 'debug':
    set_preset('debug')

    return os.system(f"python3 main.py")

  elif args[0] in ['make', 'clean']:
    return wcics_cli.indexing.main(args)

  elif args[0] == 'test':
    set_preset('testing')

    return os.system("pytest --capture=no wcics/tests -vv")

  raise ValueError("No valid command was specified!")

if "CSCENTER_KEYFILE" not in os.environ:
  os.environ['CSCENTER_KEYFILE'] = "/srv/cscenter/keys.json"

sys.exit(main(sys.argv[1:]))
