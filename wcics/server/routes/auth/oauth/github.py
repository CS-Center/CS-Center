# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.jwt import make_jwt, verify_jwt
from wcics.auth.manage_user import set_user
from wcics.auth.oauth.github import GithubOAuth

from wcics.consts import DOMAIN

from wcics.database.models import GithubLinks, Users

from wcics.utils.url import get_next_page

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
  if 'state' not in session:
    abort(401)

  state = request.args.get('state', '')
  sess_state = session.get('state')
  del session['state']

  if state != sess_state:
    # TODO: Error nicely
    abort(401)

  code = request.args.get('code', '')

  userinfo = github_oauth_client.get_userinfo(code)

  gid = str(userinfo.id)
  emails = userinfo.emails

  link = GithubLinks.query.filter_by(gid = gid).first()

  if link is None:
    users = Users.query.filter(Users.email.in_(emails)).all()
    used_emails = {user.email for user in users}
    options = [(email, make_jwt({
      "provider": "GitHub",
      "pid": gid,
      "email": email,
      "username": userinfo.username,
      "real_name": userinfo.real_name
    })) for email in emails]
    if users:
      link_tokens = [make_jwt({
        "provider": "GitHub",
        "pid": gid,
        "email": user.email,
        "uid": user.id
      }) for user in users]
      return render_template(
        "account/link-accounts.html",
        provider = "GitHub",
        matches = [(user.email, user, link_token) for user, link_token in zip(users, link_tokens)],
        no_signup = False,
        pid = gid,
        other_emails = [(email, token) for email, token in options if email not in used_emails]
      )
    elif len(options) > 1:
      return render_template("account/oauth-select-email.html", provider = "GitHub", next = get_next_page(), options = options)
    else:
      return redirect("/oauth-create-account/?next=%s&token=%s" % (get_next_page(), options[0][1]))
  else:
    set_user(Users.query.filter_by(id = link.uid).first_or_404())
    flash("Welcome back!", category = "SUCCESS")
    data = verify_jwt(state)
    print(data.get("next", "/"))
    return redirect(data.get("next", "/"), code = 303)
