# -*- coding: utf-8 -*-

from wcics import app, db

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.database.models.attendance import AttendanceCodes
from wcics.database.models.roles import AttendanceRoles, OrganizationRoles
from wcics.database.models import Organizations, OrganizationUsers
from wcics.database.utils import db_commit

from wcics.server.forms.forms.admin.attendance_edit import AttendanceEditForm
from wcics.server.forms.forms.blank_form import BlankForm

from wcics.utils.files import write_file
from wcics.utils.url import get_org_id
from wcics.utils.time import to_tstamp

from flask import abort, flash, redirect, render_template, request

import json, os

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

@app.route("/organization/<oid>/admin/attendance/", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_attendance_sudo(oid):
  if user.organization_roles.attendance < AttendanceRoles.admin:
    abort(403)

  form = BlankForm()

  org = Organizations.query.filter_by(oid = oid).first_or_404()

  if form.validate_on_submit():
    if 'delete' in request.form:
      del_id = int(request.form['delete'])

      AttendanceCodes.query.filter_by(id = del_id).delete()

      db_commit()
    else:
      b = os.urandom(16)

      c = "".join(chr(97 + bc % 26) for bc in b)

      ac = AttendanceCodes.add(oid = org.id, code = c, start = 0, end = 0)

      db_commit()

      return redirect(f"/organization/{oid}/admin/attendance/{ac.id}", code = 303)

  return render_template("adminpages/attendance.html", sudo = True, active = "attendance", form = form)

@app.route("/organization/<oid>/admin/attendance/<acid>", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_attendance_edit(oid, acid):
  if user.organization_roles.attendance < AttendanceRoles.admin:
    abort(403)

  ac = AttendanceCodes.query.filter_by(id = acid).first_or_404()

  form = AttendanceEditForm(ac)

  if form.validate_on_submit():
    start_ts = to_tstamp(form.start_date.data, form.start_time.data)
    end_ts = to_tstamp(form.end_date.data, form.end_time.data)

    if start_ts > end_ts:
      flash("Start is after end!", category = "ERROR")

    else:

      code = form.code.data

      ac.code = code
      ac.start = start_ts
      ac.end = end_ts

      db_commit()

      flash("Attendance code updated!", category = "SUCCESS")

  return render_template("adminpages/attendance-edit.html", form = form)

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
