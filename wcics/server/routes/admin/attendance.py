# -*- coding: utf-8 -*-

from wcics import app, db

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.database.models.attendance import AttendanceCodes
from wcics.database.models.roles import AttendanceRoles, OrganizationRoles
from wcics.database.models import Organizations, OrganizationUsers
from wcics.database.utils import db_commit

from wcics.server.forms import BlankForm

from wcics.utils.files import write_file
from wcics.utils.url import get_org_id

from flask import abort, flash, redirect, render_template, request

import json

@app.route("/admin/attendance/")
@assert_login
def serve_attendance_sudo_home():
  links = []

  for organization in Organizations.query.all():
    if organization.id == 1:
      continue
    if OrganizationUsers.query.filter_by(oid = organization.id, uid = user.id).count() == 0:
      continue
    role = OrganizationRoles.query.filter_by(oid = organization.id, uid = user.id).first().attendance
    if role > AttendanceRoles.default:
      links.append((organization.oid, organization.name, role))

  if links == []:
    abort(403)

  return render_template("adminpages/attendance-home.html", sudo = True, active = "attendance", links = links)

@app.route("/organization/<org>/admin/attendance/", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_attendance_sudo(org):
  if user.organization_roles.attendance < AttendanceRoles.admin:
    abort(403)

  form = BlankForm()

  if form.validate_on_submit():
    submit_type = request.form['submit']

    for change in json.loads(request.form["changes"]):
      item = AttendanceCodes.query.filter_by(id = change["id"]).first()

      if change.get("delete") is True:
        AttendanceCodes.remove(item)
      else:
        for attr in ["start", "end", "code"]:
          if attr in change:
            item.__setattr__(attr, change[attr].strip() if attr == "code" else change[attr])

    if submit_type == 'save-all-create':
      AttendanceCodes.add(oid = org, code = '', start = 0, end = 0)

    db_commit()

  return render_template("adminpages/attendance.html", sudo = True, active = "attendance", form = form)

@app.route("/organization/<org>/admin/attendance/display/")
@organization_page
@assert_login
def serve_attendance_display(org):
  if user.organization_roles.attendance <= AttendanceRoles.default:
    abort(403)

  codes = AttendanceCodes.current_objs(get_org_id())

  if len(codes) == 0:
    return render_template("adminpages/attendance-display-no-codes.html", sudo = True, active = "attendance")
  elif len(codes) == 1:
    return redirect("/organization/%s/admin/attendance/display/%s" % (org, codes[0].id))
  else:
    return render_template("adminpages/attendance-display-select.html", sudo = True, active = "attendance", codes = codes)

@app.route("/organization/<org>/admin/attendance/display/<int:cid>")
@organization_page
@assert_login
def serve_attendance_code_display(org, cid):
  if user.organization_roles.attendance <= AttendanceRoles.default:
    abort(403)

  return render_template("adminpages/attendance-display.html", sudo = True, active = "attendance", code = AttendanceCodes.query.filter_by(id = cid).first_or_404().code)
