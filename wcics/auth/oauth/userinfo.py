# -*- coding: utf-8 -*-

from flask import json

from wcics.auth.jwt import make_jwt

class UserInfo:
  def __init__(self, id, provider, emails, username = "", real_name = "", pfp_url = "", desc = "", raw = {}):
    self.provider = provider
    self.emails = emails
    self.username = username
    self.real_name = real_name
    self.pfp_url = pfp_url
    self.desc = desc
    self.raw = raw
    self.id = id
    
  def make_token(self):
    return make_jwt(self._dict())
  
  def _dict(self):
    return dict(
      id = self.id,
      provider = self.provider,
      emails = self.emails,
      username = self.username, 
      real_name = self.real_name,
      pfp_url = self.pfp_url,
      desc = self.desc,
      raw = self.raw
    )
  
  def __str__(self):
    return json.dumps(self._dict())