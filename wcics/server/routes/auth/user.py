# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user

from wcics.database.models import Users

from wcics.utils.routes import error_page

from flask import redirect, render_template

@app.route("/user/")
@assert_login
def serve_own_user():
  return get_user_page(user, user.id)

@app.route("/user/<uid>")
def serve_user(uid):
  try:
    uid = int(uid)
  except:
    return error_page(404, message = "/user must be loaded with the user ID.")
  displayuser = Users.query.filter_by(id = uid).first()
  if not displayuser: 
    return error_page(404, message = "There is no user with this ID.")
  return get_user_page(displayuser, uid)

def get_user_page(displayuser, uid):
  return render_template(
    "account/user.html",
    active = "Users",
    default_desc = "You have not provided any information about yourself." if user and user.id == uid else displayuser.username + " has not provided any information about themselves.",
    desc = displayuser.desc,
    username = displayuser.username,
    pname = "My Profile" if user and user.id == uid else displayuser.username + "'s Profile",
    solved_problems = [], # TODO
    target = displayuser
  )