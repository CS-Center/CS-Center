# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics import consts

from wcics.database.utils import db_commit

from wcics.server.forms import AttendanceForm, flash_form_errors

from wcics.utils.url import get_organization

from flask import flash, render_template

@app.route("/organization/<org>/attendance/", methods = ["GET", "POST"])
@assert_login
@organization_page
def serve_attendance(org):
  form = AttendanceForm()
  
  if form.validate_on_submit():
    flash("Your attendance was confirmed!", category = "SUCCESS")
    
    user.attendance.last_code = form.attendance_code.data.strip()
    user.attendance.count += 1
    
    db_commit()
  
  flash_form_errors(form)
  
  return render_template("account/attendance.html", form = form, off = consts.ATTENDANCE_CODE == "") # this const is dead