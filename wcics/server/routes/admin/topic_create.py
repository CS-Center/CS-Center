# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user

from wcics.database.models import Topics
from wcics.database.models.roles import TopicRoles
from wcics.database.utils import db_commit

from wcics.server.forms import TopicSudoCreateForm, flash_form_errors

from flask import abort, flash, redirect, render_template, request

@app.route("/admin/topic-create/<int:ptid>", methods = ["GET", "POST"])
@assert_login
def serve_topic_sudo_create_request(ptid):
  if user.roles.topics <= TopicRoles.default:
    abort(403)
  
  parent_tid = ""
  
  if ptid:
    parent_tid = Topics.query.filter_by(id = ptid).first_or_404().tid
  
  form = TopicSudoCreateForm()
  
  if form.validate_on_submit():
    Topics.add(ptid = ptid or None, tid = form.tid.data, name = form.name.data, desc = form.description.data)
    db_commit()
    return redirect("/admin/topics/", code = 303)
  
  flash_form_errors(form)
  
  return render_template("adminpages/topic_create.html", sudo = True, active = "topics", form = form, ptid = parent_tid)