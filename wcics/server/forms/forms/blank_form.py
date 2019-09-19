# -*- coding: utf-8 -*-

from flask_wtf import FlaskForm

from wtforms import SubmitField

class BlankForm(FlaskForm):
  submit = SubmitField()