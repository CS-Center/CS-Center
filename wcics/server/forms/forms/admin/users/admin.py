# -*- coding: utf-8 -*-

from .moderator import UserSudoModeratorForm

from wcics.server.forms.validator_sets import *

from wtforms import RadioField

class UserSudoAdminForm(UserSudoModeratorForm):
  # IMPORT regular access controls, submit button
  role_users = RadioField("usersudo role", admin_users_admin_role_users, choices = [("default", "Default"), ("moderator", "Moderator"), ("admin", "Admin")])
  role_news = RadioField("newssudo role", admin_users_admin_role_news, choices = [("underprivileged", "Underprivileged"), ("default", "Default"), ("poster", "Poster"), ("moderator", "Moderator")])
  role_database = RadioField("databasesudo role", admin_users_admin_role_database, choices = [("default", "Default"), ("admin", "Admin")])
  role_attendance = RadioField("attendancesudo role", admin_users_admin_role_attendance, choices = [("default", "Default"), ("reader", "Reader"), ("admin", "Admin")])
  role_topics = RadioField("topicsudo role", admin_users_admin_role_topics, choices = [("default", "Default"), ("editor", "Editor"), ("admin", "Admin")])