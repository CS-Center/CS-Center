# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics import consts

from wcics.database.models import AttendanceCodes, AttendanceRecords
from wcics.database.utils import db_commit

from wcics.server.forms import AttendanceForm, flash_form_errors

from wcics.utils.time import get_time
from wcics.utils.url import get_org_id

from flask import flash, render_template

@app.route("/organization/<org>/attendance/", methods = ["GET", "POST"])
@assert_login
@organization_page
def serve_attendance(org):
  form = AttendanceForm()
  
  if form.validate_on_submit():    
    AttendanceRecords.add(
      cid = AttendanceCodes.query.filter_by(code = form.attendance_code.data.strip()).first().id,
      oid = get_org_id(),
      uid = user.id,
      time = get_time()
    )
    db_commit()
    
    flash("Your attendance was confirmed!", category = "SUCCESS")
  
  flash_form_errors(form)
  
  return render_template("account/attendance.html", form = form, off = (AttendanceCodes.query.filter_by(oid = get_org_id()).count() == 0))