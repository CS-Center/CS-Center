# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user, validate_post_csrf

from wcics.database.models import Topics
from wcics.database.models.roles import TopicRoles
from wcics.database.utils import db_commit

from collections import deque

from flask import abort

@app.route("/admin/topic-move/<int:id>/<int:ptid>", methods = ["POST"])
@validate_post_csrf
@assert_login
def move_topic(id, ptid):
  if user.roles.topics <= TopicRoles.default:
    abort(403)

  if is_ancestor(ptid, id):
    return "", 400
  
  Topics.query.filter_by(id = id).first_or_404().ptid = ptid
  
  db_commit()
  return ""

@app.route("/admin/topic-move-keep-children/<int:id>/<int:ptid>", methods = ["POST"])
@validate_post_csrf
@assert_login
def move_topic_keep_children(id, ptid):
  if user.roles.topics <= TopicRoles.default:
    abort(403)
  
  if Topics.is_parent(id, ptid):
    return "", 400
  
  topic = Topics.query.filter_by(id = id).first_or_404()
  
  for child in Topics.query.filter_by(ptid = topic.id).all():
    child.ptid = topic.ptid
  
  db_commit()
  
  topic.ptid = ptid
  
  db_commit()
  return ""