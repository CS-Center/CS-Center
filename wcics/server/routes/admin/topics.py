# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user

from wcics.database.models import Topics
from wcics.database.models.roles import TopicRoles
from wcics.database.utils import db_commit

from wcics.server.forms import BlankForm

from flask import abort, flash, render_template, request

@app.route("/admin/topics/", methods = ["GET", "POST"])
@assert_login
def serve_topic_sudo_request():
  if user.roles.topics <= TopicRoles.default:
    abort(403)

  topiclist = Topics.query.all()
  topicmap = { topic.id: topic for topic in topiclist }
  rootids = [topic.id for topic in sorted([topic for topic in topiclist if not topic.ptid], key = lambda topic: topic.tid)]
  children = { topic.id: [] for topic in topiclist }
  
  for topic in topiclist:
    if topic.ptid:
      children[topic.ptid].append(topic.id)
  
  for topic in topiclist:
    children[topic.id].sort(key = lambda id: topicmap[id].tid)
  
  form = BlankForm()
  
  return render_template("adminpages/topics.html", sudo = True, active = "topics", topiclist = topiclist, topicmap = topicmap, rootids = rootids, children = children, form = form)