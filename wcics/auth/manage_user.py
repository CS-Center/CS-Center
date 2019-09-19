# -*- coding: utf-8 -*-

from .consts import FRESH_SESSION_THRESHOLD, JWT_EXPIRY

from .cookies import set_cookie

from .jwt import make_jwt, verify_jwt, InvalidJWT, ExpiredJWT

from werkzeug.local import Local

from wcics import app

from wcics.database.models import Users, Organizations, OrganizationUsers
from wcics.database.models.roles import OrganizationAdminRoles

from wcics.server.errors import NotLoggedInError, SessionNotFreshError

from wcics.utils.routes import error_page
from wcics.utils.time import get_time
from wcics.utils.url import get_organization

from flask import redirect, request, session

import base64

# Setup a thread proxy object
user_manager = Local()
user = user_manager('user')

# Utility to set the global user object
def set_user(obj, update = True):
  user_manager.user = obj
  user_manager._update = getattr(user_manager, 'update', False) or update

# Resolve the user before the request
@app.before_request
def resolve_user():
  # Dont load the user if the endpoint is static
  if request.endpoint == 'static':
    return
  
  # Set it to None by default
  set_user(None, False)
  
  try:
    # Read cookie
    u_cookie = request.cookies.get("user", "")
    if u_cookie:
      token = verify_jwt(u_cookie)
      
      if "uid" not in token:
        return
      
      u = Users.query.filter_by(id = token["uid"]).first()
            
      if u:
        if get_time() >= u.permissions.can_login_after and u.permissions.can_login_after != -1 and "iat" in token and token['iat'] >= u.permissions.revoke_tokens_before:
          set_user(u, False)
      
  except (InvalidJWT, ExpiredJWT):
    pass

def organization_page(view_func):
  def _inner(*args, **kwargs):
    org = get_organization()
    organization = Organizations.query.filter_by(oid = org).first()
    if organization is None:
      return error_page(404, "This organization does not exist!")
    if org != "main":
      # The main organization is accessible to everyone
      if not user:
        raise NotLoggedInError
      if user.roles.organizations <= OrganizationAdminRoles.default:
        # General organization admins can visit any organization's page
        if OrganizationUsers.query.filter_by(oid = organization.id, uid = user.id).count() == 0:
          return redirect("/organization/%s/landing" % org, code = 303)
    return view_func(*args, **kwargs)
  _inner.__name__ = view_func.__name__
  return _inner

def assert_login(view_func):
  def _inner(*args, **kwargs):
    if not user:
      raise NotLoggedInError
    return view_func(*args, **kwargs)
  _inner.__name__ = view_func.__name__
  return _inner

def sensitive_action(view_func):
  def _inner(*args, **kwargs):
    if get_time() - verify_jwt(request.cookies.get("user"))["iat"] > FRESH_SESSION_THRESHOLD:
      raise SessionNotFreshError
    return view_func(*args, **kwargs)
  _inner.__name__ = view_func.__name__
  return assert_login(_inner)

def validate_post_csrf(view_func):
  def _inner(*args, **kwargs):
    if app.testing or request.method == "GET":
      return view_func(*args, **kwargs)
    # So, flask-wtf is a weird and it decided to take the CSRF token, PUT QUOTATION MARKS AROUND IT, and then sign it into a JWT
    # so yeah
    # 
    # wtf
    valid = False
    try:
      valid = session.get("csrf_token") == base64.b64decode(request.json.get("token", "").split(".")[0]).decode("utf-8")[1:-1]
    except:
      pass
    if valid:
      return view_func(*args, **kwargs)
    return "", 400
  _inner.__name__ = view_func.__name__
  return _inner

def mint_token(uid):
  # Mint a new token. 
  # Sign with the jwt_key
  # Expires in 1 month (30 days)
  # This is the signed cookie sent to the server for authentication. 
  return make_jwt(dict(
    exp = get_time() + JWT_EXPIRY,
    uid = uid,
    iat = get_time()
  ))
  
@app.after_request
def set_user_cookie(resp):
  if not getattr(user_manager, '_update', False):
    return resp
  
  if not user:
    set_cookie(resp, 'user', '')
  
  else:
    set_cookie(resp, 'user', mint_token(user.id))
    
  return resp
