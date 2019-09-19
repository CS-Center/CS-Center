# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import user

from wcics.server.consts import USERS_PER_PAGE
from wcics.server.routes.utils import paged_data

from wcics.database.models import Users

from flask import render_template

@app.route("/users/")
def serve_user_home():
  page, pages, users = paged_data(Users.query.order_by(Users.username).all(), USERS_PER_PAGE)
  return render_template("account/users.html", active = "Users", page = page, pages = pages, users = users)