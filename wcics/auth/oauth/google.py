# -*- coding: utf-8 -*-

from .userinfo import UserInfo

from cachecontrol import CacheControl

from flask import json, redirect

from urllib.parse import urlencode

from wcics import app

import requests

class GoogleOAuth:
  def __init__(self, redirect_uri, scope):
    self._session = CacheControl(requests.Session())
    
    self.client_id = app.config['GOOGLE_OAUTH_CLIENT_ID']
    self.client_secret = app.config['GOOGLE_OAUTH_CLIENT_SECRET']
    
    self.redirect_uri = redirect_uri
    self.scope = " ".join(scope)
    
  def __getitem__(self, key):
    return self._session.get("https://accounts.google.com/.well-known/openid-configuration").json()[key]
  
  def authorize_user(self, state):
    url = self["authorization_endpoint"]
    
    params = dict(
      client_id = self.client_id,
      response_type = 'code',
      scope = self.scope,
      redirect_uri = self.redirect_uri,
      state = state,
      prompt = 'select_account',
    )
    
    return redirect(url + "?" + urlencode(params))
  
  def get_userinfo(self, code):
    url = self["token_endpoint"]
    
    resp = self._session.post(
      url,
      auth = (self.client_id, self.client_secret),
      json = dict(
        code = code, 
        grant_type = 'authorization_code',
        redirect_uri = self.redirect_uri
      )
    )
    
    acc_token = resp.json()['access_token']
    
    url = self["userinfo_endpoint"]
    
    resp = self._session.get(
      url,
      headers = dict(
        Authorization = 'Bearer {}'.format(acc_token)
      )
    )
    
    info = resp.json()
    
    id = int(info['sub'])
    
    email = info['email']
    assert info['email_verified']
    
    name = info.get('name', '')
    
    pfp_url = info.get('picture', '')
    
    return UserInfo(id, 'Google', [email], username = name, pfp_url = pfp_url, raw = info)