# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField

class AttendanceForm(FlaskForm):
  attendance_code = StringField("Attendance Code", account_attendance_attendance_code)
  submit = SubmitField("Submit")