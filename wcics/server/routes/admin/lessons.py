# -*- coding: utf-8 -*-

from wcics import app

from flask import abort, redirect, render_template

@app.route("/admin/lessons/")
@assert_login
def serve_news_sudo_home():  
  links = [(organization.oid, organization.name) for organization in user.lesson_admin_organizations()]
  
  if links == []:
    abort(403)
  
  return render_template("adminpages/news-home.html", sudo = True, active = "news", links = links)