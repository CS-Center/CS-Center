# -*- coding: utf-8 -*-

from flask import render_template, request, json
from flask_socketio import send, emit

from wcics import app, socketio
from wcics.auth.manage_user import user

@app.route("/testing")
def test_route():
  return render_template("utils/test-template.html")

@app.route("/test-request")
def test_req():
  return repr(request.__dict__)

@socketio.on('connect')
def handle_my_custom_event():
  if 'user' not in request.cookies:
    return False
  print(user)
  emit('connection-established', "A Client Connected!", broadcast = True)