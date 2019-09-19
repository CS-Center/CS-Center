# -*- coding: utf-8 -*-

from wcics.server.consts import NO_REDIRECT

from wcics.database.models.organizations import Organizations

from flask import request

import re

def get_next_page():
  nxt = request.args.get("next", "/")
  if not nxt.startswith("/"):
    return "/external?url=%s" % nxt
  if nxt in NO_REDIRECT:
    return "/"
  return nxt

def get_organization():
  match = re.search("^/(o|org|organization)/([^/]+)", request.path)
  if match:
    return match.group(2)
  return "main"

def get_org_id():
  return Organizations.query.filter_by(oid = get_organization()).first().id