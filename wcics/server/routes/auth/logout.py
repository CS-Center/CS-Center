# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import set_user

from wcics.utils.url import get_next_page

from flask import redirect, flash

@app.route("/logout/")
def logout():
  set_user(None)
  flash("You are now logged out. See you later!", category = "SUCCESS")
  return redirect(get_next_page(), code = 303)