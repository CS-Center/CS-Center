from .utils import error
import os

ports = dict(
  keenan = 4000,
  alex = 5000
)

alt_names = dict(
  keenan = 'riolku',
  alex = 'neutrino'
)

FQDN = "wcics.club"

def setup(args):
  if not args:
    return error("You must provide a configuration to load!")
  
  USERNAME = os.environ['WCICS_USERNAME']
  BRANCH = os.environ['WCICS_BRANCH']
  DOMAIN = alt_names[USERNAME] + "." + FQDN
  
  if args[0] not in ['inspect', 'debug', 'production', 'testing', 'prod-test']:
    return error("Unrecognized configuration: '%s'" % args[0])
    
  os.environ['WCICS_PRESET'] = args[0]
  
  if args[0] == 'inspect':
    pass
  
  elif args[0] == 'debug':
    PORT = ports[USERNAME]
  
  elif args[0] == 'production':
    DOMAIN = FQDN
    PORT = 0
    
  elif args[0] == 'prod-test':
    DOMAIN = 'prod-test.' + FQDN
    PORT = 0
  
  elif args[0] == 'testing':
    PORT = 6000
  
  if port != 0:
    os.system("fuser -k {port}/tcp -s".format(port = PORT))
  
  os.environ['WCICS_DOMAIN'] = DOMAIN
  os.environ['WCICS_PORT'] = str(PORT)