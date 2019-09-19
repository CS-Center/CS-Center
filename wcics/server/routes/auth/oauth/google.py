# -*- coding: utf-8 -*-

# Oauth control for google

from wcics import app

from wcics.auth.jwt import make_jwt, verify_jwt
from wcics.auth.manage_user import set_user
from wcics.auth.oauth.google import GoogleOAuth

from wcics.consts import DOMAIN

from wcics.database.models import GoogleLinks, Users

from wcics.utils.url import get_next_page

from flask import abort, flash, json, redirect, render_template, request, session

import base64, os

google_oauth_client = GoogleOAuth(
  'https://{}/authorized/google/'.format(DOMAIN),
  ['openid', 'https://www.googleapis.com/auth/userinfo.profile', 'https://www.googleapis.com/auth/userinfo.email']
)

@app.route("/login/google/")
def login_google():
  next = request.args.get("next", "/")
  nonce = base64.b64encode(os.urandom(24)).decode('utf-8')
  state = make_jwt(dict(
    next = next,
    nonce = nonce
  ))

  session['state'] = state

  return google_oauth_client.authorize_user(state)

@app.route("/authorized/google/")
def authorize_google():
  if 'state' not in session:
    abort(401)

  state = request.args.get('state', '')
  sess_state = session.get('state')
  del session['state']

  if state != sess_state:
    # TODO: Error nicely
    abort(401)

  code = request.args.get('code', '')

  userinfo = google_oauth_client.get_userinfo(code)

  gid = str(userinfo.id)
  email = userinfo.raw["email"]

  link = GoogleLinks.query.filter_by(gid = gid).first()

  if link is None:
    user = Users.query.filter_by(email = email).first()
    if user is not None:
      link_token = make_jwt({
        "provider": "Google",
        "pid": gid,
        "email": email,
        "uid": user.id
      })
      return render_template("account/link-accounts.html", provider = "Google", matches = [(email, user, link_token)], no_signup = True, pid = gid)
    else:
      connect_token = make_jwt({
        "provider": "Google",
        "pid": gid,
        "email": email,
        "real_name": userinfo.raw["name"]
      })
      return redirect("/oauth-create-account/?next=%s&token=%s" % (get_next_page(), connect_token))
  else:
    set_user(Users.query.filter_by(id = link.uid).first_or_404())
    flash("Welcome back!", category = "SUCCESS")
    data = verify_jwt(state)
    return redirect(data.get("next", "/"), code = 303)
