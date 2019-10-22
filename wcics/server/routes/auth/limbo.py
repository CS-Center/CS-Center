# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import user

from flask import redirect, render_template, request

from wcics.utils.url import get_next_page

@app.route("/signup-limbo/")
def serve_signup_limbo():
  if "email" not in request.args or user:
    return redirect(get_next_page(), code = 303)
  return render_template("account/signup-limbo.html", active = "Sign Up", email = request.args.get("email", ""))

@app.route("/reset-limbo/")
def serve_reset_limbo():
  if "email" not in request.args or user:
    return redirect(get_next_page(), code = 303)
  return render_template("account/reset-limbo.html", active = "Log In", email = request.args.get("email", ""))