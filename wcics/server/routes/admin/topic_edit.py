# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user

from wcics.database.models import Topics
from wcics.database.models.roles import TopicRoles
from wcics.database.utils import db_commit

from wcics.server.forms import TopicSudoEditForm, flash_form_errors

from flask import abort, flash, redirect, render_template, request

@app.route("/admin/topic-edit/<int:id>", methods = ["GET", "POST"])
@assert_login
def serve_topic_sudo_edit_request(id):
  if user.roles.topics <= TopicRoles.default:
    abort(403)
  
  topic = Topics.query.filter_by(id = id).first_or_404()
  
  form = TopicSudoEditForm(topic)
  
  if form.validate_on_submit():
    topic.tid = form.tid.data
    topic.name = form.name.data
    topic.desc = form.description.data
    db_commit()
    flash("Successfully updated topic!", category = "SUCCESS")
  
  flash_form_errors(form)
  
  if form.tid.data is None:
    form.tid.data = topic.tid
  
  if form.name.data is None:
    form.name.data = topic.name
  
  if form.description.data is None:
    form.description.data = topic.desc
  
  return render_template("adminpages/topic_edit.html", sudo = True, active = "topics", form = form)