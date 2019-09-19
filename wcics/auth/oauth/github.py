# -*- coding: utf-8 -*-

from .userinfo import UserInfo

from wcics import app

from flask import json, redirect

from urllib.parse import urlencode

import requests

class GithubOAuth:
  def __init__(self, redirect_uri, scope):
    self.redirect_uri = redirect_uri
    self.scope = " ".join(scope)
    self.client_id = app.config['GITHUB_OAUTH_CLIENT_ID']
    self.client_secret = app.config['GITHUB_OAUTH_CLIENT_SECRET']
    
  def authorize_user(self, state):
    url = "https://github.com/login/oauth/authorize"
    
    params = dict(
      client_id = self.client_id,
      redirect_uri = self.redirect_uri,
      scope = self.scope,
      state = state,
    )
    
    return redirect(url + "?" + urlencode(params))
  
  def get_userinfo(self, code):
    url = "https://github.com/login/oauth/access_token"
    
    resp = requests.post(
      url,
      headers = dict(
        Accept = 'application/json'
      ),
      json = dict(
        client_id = self.client_id,
        client_secret = self.client_secret,
        code = code,
      )
    )
        
    tkn = resp.json()['access_token']
    
    url = "https://api.github.com/user"
    
    resp = requests.get(
      url,
      headers = dict(
        Authorization = "token {}".format(tkn),
        Accept = "application/vnd.github.v3+json"
      )
    )
    
    profile = resp.json()
    
    url = "https://api.github.com/user/emails"
    
    resp = requests.get(
      url,
      headers = dict(
        Authorization = "token {}".format(tkn),
        Accept = "application/vnd.github.v3+json"
      )
    )
    
    emails = resp.json()
    
    id = profile['id']
    
    primary_email = None
    
    for email_info in emails:
      if email_info["primary"] and email_info["verified"]:
        primary_email = email_info["email"]
    
    other_emails = [
      email_info["email"] for email_info in emails 
      if email_info["verified"] and not email_info["primary"]
        and not email_info["email"].endswith("users.noreply.github.com")
    ]
    
    assert primary_email is not None
    
    username = profile.get("login", "")
    pfp_url = profile.get("avatar_url", "")
    real_name = profile.get("name")
    if real_name is None:
      real_name = ""
      
    desc = profile.get("bio")
    if desc is None:
      desc = ""
    
    raw_data = profile
    raw_data['emails'] = emails
    
    return UserInfo(id,
                    "GitHub", 
                    [primary_email] + other_emails,
                    username = username,
                    real_name = real_name,
                    pfp_url = pfp_url,
                    desc = desc,
                    raw = raw_data
                   )