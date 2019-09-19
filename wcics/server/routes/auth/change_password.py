# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import sensitive_action, set_user, user
from wcics.auth.hash import pass_hash

from wcics.database.utils import db_commit

from wcics.server.consts import ERROR_MESSAGES
from wcics.server.errors import RedirectError
from wcics.server.forms import ChangePasswordForm, flash_form_errors

from wcics.utils.time import get_time

from flask import flash, redirect, render_template, request

@app.route("/change-password/", methods = ["GET", "POST"])
@sensitive_action
def serve_change_password_request():
  form = ChangePasswordForm()

  if form.validate_on_submit():
    return serve_change_password(form)
  
  flash_form_errors(form)
  return render_template("account/change-password.html", active = "Account", form = form)

def serve_change_password(form):
  password = form.password.data
  user.password_hash = pass_hash(password, user.salt)
  user.permissions.revoke_tokens_before = get_time()
  db_commit()
  
  set_user(user._get_current_object())
  flash("Your account password has been updated!", category = "SUCCESS")
  return redirect("/edit-profile/", code = 303)