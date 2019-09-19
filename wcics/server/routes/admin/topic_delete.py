# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user, validate_post_csrf

from wcics.database.models import Topics
from wcics.database.models.roles import TopicRoles
from wcics.database.utils import db_commit

from collections import deque

from flask import abort

@app.route("/admin/topic-delete/<int:id>", methods = ["POST"])
@validate_post_csrf
@assert_login
def delete_topic(id):
  if user.roles.topics <= TopicRoles.default:
    abort(403)

  Topics.remove(Topics.query.filter_by(id = id).first_or_404())
  
  db_commit()
  return ""

@app.route("/admin/topic-delete-keep-children/<int:id>", methods = ["POST"])
@validate_post_csrf
@assert_login
def delete_topic_keep_children(id):
  if user.roles.topics <= TopicRoles.default:
    abort(403)
  
  topic = Topics.query.filter_by(id = id).first_or_404()
  
  for child in Topics.query.filter_by(ptid = topic.id).all():
    child.ptid = topic.ptid
  
  db_commit()
  
  Topics.remove(topic)
  
  db_commit()
  return ""