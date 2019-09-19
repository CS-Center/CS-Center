# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import user

from wcics.consts import STATIC_FOLDER_PATH

from wcics.database.models import Users

from flask import send_file

import os

@app.route("/pfp/<id>")
def serve_pfp(id):
  # Query the user by id
  user = Users.query.filter_by(id = id).first_or_404()
  
  # Return the image if the user exists
  if os.path.exists("/root/pfp/%s" % user.id):
    return send_file("/root/pfp/%s" % user.id, mimetype = "image/png")

  # If they don't have a pfp, serve the default
  return send_file(STATIC_FOLDER_PATH + "/images/default-pfp.png")