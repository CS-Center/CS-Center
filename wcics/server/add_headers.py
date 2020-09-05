# -*- coding: utf-8 -*-

from wcics import app

from wcics.config.consts import CSP_HEADERS

# Add security headers to responses
# This will get moved to nginx when we deploy production

@app.after_request
def add_security_headers(resp):
  csp_policy = ""
  for key, val in CSP_HEADERS.items():
    csp_policy += key + " " + " ".join(val) + "; "

  resp.headers['Content-Security-Policy'] = csp_policy
  return resp
