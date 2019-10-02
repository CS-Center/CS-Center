# -*- coding: utf-8 -*-

from wcics import app

from wcics.utils.routes import error_page

from flask import request

@app.route("/error/<int:code>")
def serveError(code):
  return error_page(code)

@app.errorhandler(403)
def serve403(e):
  return error_page(403)
  
@app.errorhandler(404)
def serve404(e):
  return error_page(404)

@app.errorhandler(500)
def serve500(e):
  return error_page(500)