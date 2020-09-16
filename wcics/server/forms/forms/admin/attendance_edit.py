# -*- coding: utf-8 -*-

from flask_wtf import FlaskForm

from wtforms import SubmitField, TextField, SelectField
from wtforms.fields.html5 import DateField, TimeField

from wcics.server.forms.validator_sets import admin_attendance_attendance_code

from wcics.utils.time import split_ts

class AttendanceEditForm(FlaskForm):
  def __init__(self, ac):
    FlaskForm.__init__(self, obj = ac)

    if self.start_date.data is None and self.start_time.data is None:
      self.start_date.data, self.start_time.data = split_ts(ac.start)

    if self.end_date.data is None and self.end_time.data is None:
      self.end_date.data, self.end_time.data = split_ts(ac.end)

    self.assoc_obj = ac

  code = TextField("Code", admin_attendance_attendance_code)

  start_date = DateField('Start Date', format = "%Y-%m-%d")
  start_time = TimeField('Start Time', format = "%H:%M")

  end_date = DateField('End Date', format = "%Y-%m-%d")
  end_time = TimeField('End Time', format = "%H:%M")

  submit = SubmitField("Update")
