# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user

from wcics.database.models.roles import AttendanceRoles, DatabaseRoles, NewsRoles, TopicRoles, UserRoles

from flask import abort, redirect, render_template

@app.route("/admin/")
@assert_login
def serve_admin_root():
  accessible = []
  if user.roles.users > UserRoles.default:
    accessible.append(("users", "manage user permissions"))
  if user.roles.database > DatabaseRoles.default:
    accessible.append(("database", "on-site database queries"))
  if False: # TODO
    accessible.append(("news", "post/manage news items"))
  if False: # TODO
    accessible.append(("attendance", "read/write the attendance code"))
  if False: # TODO
    accessible.append(("topics", "create/edit lesson and problem topics"))
  return render_template("adminpages/admin.html", sudo = True, active = "admin", accessible = accessible)