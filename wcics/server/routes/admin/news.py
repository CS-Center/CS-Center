# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user

from wcics.database.models import News
from wcics.database.models.roles import NewsRoles

from flask import abort, redirect, render_template

@app.route("/admin/news/")
@assert_login
def serve_news_sudo():
  if user.roles.news <= NewsRoles.default:
    abort(403)
  
  return render_template("adminpages/news.html", sudo = True, active = "news", news = News.query.order_by(News.time.desc()).all())