# -*- coding: utf-8 -*-

from wcics import app
from wcics.auth.manage_user import assert_login, user

from wcics.database.models import Users
from wcics.database.models.roles import UserRoles
from wcics.database.models.roles.consts import roles
from wcics.database.utils import db_commit, db_rollback

from wcics.server.forms import UserSudoAdminForm, UserSudoModeratorForm, flash_form_errors

from wcics.utils.routes import error_page

from flask import abort, flash, json, redirect, render_template, request

@app.route("/admin/user/<uid>", methods = ["GET", "POST"])
@assert_login
def serve_user_sudo_edit_request(uid):
  
  if user.roles.users <= UserRoles.default:
    abort(403)
  
  try:
    uid = int(uid)
  except:
    return error_page(404, message = "/admin/user must be loaded with the user ID.")
  
  target = Users.query.filter_by(id = uid).first()
  
  if not target:
    return error_page(404, message = "There is no user with this ID.")
  
  if UserRoles.admin > user.roles.users <= target.roles.users:
    abort(403)
  
  form = UserSudoAdminForm() if user.roles.users >= UserRoles.admin else UserSudoModeratorForm()
  
  if form.validate_on_submit():
    flash("Successfully updated user!", category = "SUCCESS")
    user_sudo_edit(target, form)
  else:
    flash_form_errors(form, "Changes were not saved!")
    
  return render_template("adminpages/user.html", sudo = True, active = "users", target = target, form = form)

def user_sudo_edit(target, form):
  if user.roles.users >= UserRoles.moderator:
    target.permissions.can_login_after = form.can_login_after.data
    
    if user.roles.users >= UserRoles.admin:
      for role in ["users", "database", "news", "attendance", "topics"]:
        setattr(target.roles, role, getattr(getattr(roles, role), getattr(form, "role_%s" % role).data))
  
    db_commit()