# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import user

from wcics.database.models import Users

from flask import send_file

import os

@app.route("/pfp/<id>")
def serve_pfp(id):
  # Query the user by id
  user = Users.query.filter_by(id = id).first_or_404()

  # If they don't have a pfp, serve the default
  return send_file("/static/images/default-pfp.png")
