# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user, validate_post_csrf

from wcics.database.utils import db_commit

from wcics.utils.time import get_time

from flask import request

import base64

@app.route("/global-logout", methods = ["POST"])
@validate_post_csrf
@assert_login
def global_logout():
  user.permissions.revoke_tokens_before = get_time()
  db_commit()
  return ""