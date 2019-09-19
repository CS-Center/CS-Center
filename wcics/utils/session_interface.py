# -*- coding: utf-8 -*-

import flask
from wcics.auth._jwt import make_jwt, verify_jwt
from wcics.auth._cookies import set_cookie

class WCICS_Session(dict, flask.sessions.SessionMixin):
  pass

class WCICS_Session_Interface(flask.sessions.SessionInterface):
  def open_session(self, app, request):
    key = app.secret_key
            
    token = request.cookies.get("session", "")
        
    try:
      return WCICS_Session(
        verify_jwt(token, key)
      )
    except Exception as e:
      return WCICS_Session({
        
      })
    
  def save_session(self, app, session, response):
    key = app.secret_key
    
    set_cookie(app.config, response, "session", make_jwt(session, key))