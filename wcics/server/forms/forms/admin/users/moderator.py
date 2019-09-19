# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import IntegerField, SubmitField

class UserSudoModeratorForm(FlaskForm):
  can_login_after = IntegerField("Suspend login access until", admin_users_moderator_can_login_after)
  submit = SubmitField("Update User")