import os, sys

def set_constant(k, v):
  globals()[k] = v

PRESET = os.environ['CSCENTER_PRESET']

PORT = os.environ['CSCENTER_PORT']

DOMAIN = os.environ['CSCENTER_DOMAIN']

LOCAL = DOMAIN.startswith('local')

CSP_HEADERS = None
