# -*- coding: utf-8 -*-

from wcics import app, db, md

from wcics.auth.manage_user import assert_login, user

from wcics.database.models import News, NewsAuthors, Users
from wcics.database.models.roles import NewsRoles
from wcics.database.utils import db_commit

from wcics.mail.utils import send_many

from wcics.server.forms import NewsSudoCreateForm, flash_form_errors

from wcics.utils.time import get_time

from flask import abort, flash, redirect, render_template

import time

@app.route("/admin/news-create/", methods = ["GET", "POST"])
@assert_login
def serve_news_sudo_create_request():
  if user.roles.news <= NewsRoles.default:
    abort(403)

  form = NewsSudoCreateForm()
    
  if form.validate_on_submit():
    flash("Successfully created news item!", category = "SUCCESS")
    news_sudo_create(form)
    return redirect("/admin/news/", code = 303)
  else:
    flash_form_errors(form)
  
  return render_template("adminpages/news-create.html", sudo = True, active = "news", form = form)

def news_sudo_create(form):
  News.add(nid = form.nid.data, title = form.title.data, body = form.body.data, time = get_time())
  db_commit()
  
  for uid in form.authors.data.split():
    NewsAuthors.add(nid = form.nid.data, uid = int(uid))
  
  if form.email.data:
    send_many([tup[0] for tup in db.session.query(Users.email).filter(Users.subscribed == True).all()], "CS Center Announcement - %s" % form.title.data, md.render(form.body.data))
  
  db_commit()