import os, sys

PRESET = os.environ['CSCENTER_PRESET']

DOMAINS = {
  4000 : 'riolku.cscenter.ca',
  5000 : 'neutrino.cscenter.ca'
}

PORT = int(sys.argv[1])

DOMAIN = DOMAINS[PORT]

CSP_HEADERS = None
