# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.jwt import verify_jwt
from wcics.auth.manage_user import set_user

from wcics.database.models import GoogleLinks, GithubLinks, Users
from wcics.database.utils import db_commit

from wcics.server.forms import BlankForm, flash_form_errors

from flask import flash, redirect, render_template, request

@app.route("/link-account/", methods = ["GET", "POST"])
def link_account():
  data = verify_jwt(request.args.get("token", ""))
  form = BlankForm()

  uid = data["uid"]
  pid = data["pid"]
  email = data["email"]
  provider = data["provider"]

  if form.validate_on_submit():
    if provider == "Google":
      GoogleLinks.add(uid = uid, gid = pid)
    elif provider == "GitHub":
      GithubLinks.add(uid = uid, gid = pid)
    db_commit()
    set_user(Users.query.filter_by(id = uid).first())
    flash("Your account is now connected. Welcome back!", category = "SUCCESS")
    return redirect("/", code = 303)
    # TODO GitHub

  flash_form_errors(form)

  return render_template("account/link-account.html", uid = uid, provider = provider, email = email, form = form)
