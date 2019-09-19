# -*- coding: utf-8 -*-

from wcics import app, db

from wcics.auth.manage_user import assert_login, user

from wcics.database.models import Users
from wcics.database.models.roles import UserRoles

from flask import abort, redirect, render_template

@app.route("/admin/users/")
@assert_login
def serve_user_sudo():
  if user.roles.users <= UserRoles.default:
    abort(403)
    
  return render_template("adminpages/users.html", sudo = True, active = "users")