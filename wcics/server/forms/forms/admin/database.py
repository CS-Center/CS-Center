# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import SubmitField, TextAreaField

class DatabaseSudoForm(FlaskForm):
  query = TextAreaField("SQL Query", admin_database_query)
  submit = SubmitField("Submit Query")