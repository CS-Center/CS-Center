# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import user

from wcics.auth.actions import reset_password as send_reset_email

from wcics.server.forms import ResetPasswordForm, flash_form_errors

from wcics.utils.url import get_next_page

from flask import redirect, render_template

@app.route("/reset-password/", methods = ["GET", "POST"])
def reset_password():
  if user:
    return redirect(get_next_page(), code = 303)
  
  form = ResetPasswordForm()
  
  if form.validate_on_submit():
    email = form.email.data
    
    send_reset_email(email)
    return redirect("/reset-limbo/?email=%s" % email)
  
  flash_form_errors(form)
  return render_template("account/reset-password.html", active = "Log In", form = form)