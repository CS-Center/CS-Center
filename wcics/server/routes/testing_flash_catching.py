# -*- coding: utf-8 -*-

from wcics import app

from flask import get_flashed_messages, make_response, json

if app.testing:
  @app.after_request
  def check_flashes(response):
    fm = get_flashed_messages(False, "ERROR")
    if fm:
      res = make_response(json.dumps(fm), 406, {'content-type' : 'application/json'})
      return res
    else:
      return response