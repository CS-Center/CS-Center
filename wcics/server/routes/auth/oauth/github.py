# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.jwt import make_jwt, verify_jwt, InvalidJWT, ExpiredJWT
from wcics.auth.manage_user import set_user, user
from wcics.auth.oauth.github import GithubOAuth

from wcics.consts import DOMAIN

from wcics.database.models import GithubLinks, Users

from wcics.utils.url import get_next_page
from wcics.utils.routes import error_page 

from flask import abort, flash, json, redirect, render_template, request, session

import base64, os

github_oauth_client = GithubOAuth(
  "https://{}/authorized/github/".format(DOMAIN),
  ["user:email", "read:user"]
)

@app.route("/login/github/")
def login_github():
  next = request.args.get("next", "/")
  nonce = base64.b64encode(os.urandom(24)).decode('utf-8')
  state = make_jwt(dict(
    next = next,
    nonce = nonce
  ))

  session['state'] = state

  return github_oauth_client.authorize_user(state)

@app.route("/authorized/github/")
def authorize_github():  
  if user is not None:
    next_url = request.args.get("next")
    if next_url is None:
      try:
        if 'state' in session:
          state = session['state']
        else:
          state = request.args.get("state", "")
        
        data = verify_jwt(state)
        
        next_url = data.get("next", "/")
      except (InvalidJWT, ExpiredJWT):
        next_url = "/"
    
    return redirect(next_url, code = 303)
  
  if 'state' not in session:
    return error_page(400, message = "No state was provided! Please return to /login to retrieve a valid state.")

  state = request.args.get('state', '')
  sess_state = session.get('state')
  
  del session['state']
    
  if state != sess_state:
    return error_page(400, message = "The provided state is invalid! Please return to /login to retrieve a new state.")
  
  try:
    next_url = verify_jwt(sess_state).get("next", "/")
  except (InvalidJWT, ExpiredJWT):
    return error_page(400, message = "The provided state is invalid! Please return to /login to retrieve a new state.")

  code = request.args.get('code', '')

  userinfo = github_oauth_client.get_userinfo(code)

  gid = str(userinfo.id)
  email = userinfo.raw["email"]

  link = GithubLinks.query.filter_by(gid = gid).first()

  if link is None:
    link_user = Users.query.filter_by(email = email).first()
    if link_user is not None:
      link_token = make_jwt({
        "provider": "Github",
        "pid": gid,
        "email": email,
        "uid": link_user.id
      })
      return render_template("account/link-accounts.html", provider = "Github", matches = [(email, link_user, link_token)], no_signup = True, pid = gid)
    else:
      connect_token = make_jwt({
        "provider": "Github",
        "pid": gid,
        "email": email,
        "real_name": userinfo.raw["name"]
      })
      return redirect("/oauth-create-account/?next=%s&token=%s" % (next_url, connect_token))
  else:
    set_user(Users.query.filter_by(id = link.uid).first_or_404())
    flash("Welcome back!", category = "SUCCESS")
    return redirect(next_url, code = 303)
