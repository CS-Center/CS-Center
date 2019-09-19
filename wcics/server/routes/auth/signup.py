# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.actions import link_email, verify_email
from wcics.auth.manage_user import user

from wcics.database.models import Users

from wcics.server.forms import SignupForm, flash_form_errors

from wcics.utils.url import get_next_page

from flask import flash, redirect, render_template, request

@app.route("/signup/", methods = ["GET", "POST"])
def serve_signup_request():
  if user:
    return redirect("/", code = 303)
  
  form = SignupForm()

  if form.validate_on_submit():
    email = form.email.data.strip()
    if Users.query.filter_by(email = email).count() > 0:
      link_email(email, get_next_page())
    else:
      verify_email(email, get_next_page())
    return redirect("/signup-limbo/?email=%s" % email, code = 303)
  
  flash_form_errors(form)
  return render_template("account/signup.html", active = "Sign Up", next_page = get_next_page(), form = form)