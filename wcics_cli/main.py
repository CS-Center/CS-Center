# Main CLI file

import sys

from wcics_cli.start import main as start_main
from wcics_cli.inspection import main as inspect_main
from wcics_cli.utils import error

def main(args):
  if not args:
    return error("Please provide a subcommand.")
  
  if args[0] == 'start':
    return start_main(args[1:])
  
  elif args[0] == 'inspect':
    return inspect_main(args[1:])
  
  else:
    return error("Unrecognized subcommand: '%s'" % args[0])
  
if __name__ == "__main__":
  sys.exit(main(sys.argv[1:]))