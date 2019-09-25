# -*- coding: utf-8 -*-

from wcics import consts

from wcics.auth.hash import pass_hash
from wcics.auth.manage_user import user

from wcics.database.models import Organizations, AttendanceCodes, Users

from wcics.server.consts import ERROR_MESSAGES

from wcics.utils.time import get_time
from wcics.utils.url import get_org_id

from wtforms import validators, ValidationError

import string, unicodedata, validate_email

def assert_checked(message):
  def _inner(form, field):
    if not field.data:
      raise ValidationError(message)
  return _inner

attendance_code_charset = string.ascii_letters + string.digits + "-"
def attendance_code_format(form, field):
  if any(char not in attendance_code_charset for char in field.data):
    raise ValidationError(ERROR_MESSAGES["attendance_characters_invalid"])

def author_format(form, field):
  if not all(s.isdigit() for s in field.data.split()):
    raise ValidationError(ERROR_MESSAGES["authors_format_invalid"])

def check_unique(db, param, unique = True, name = None, message = None):
  def _inner(form, field):
    obj = db.query.filter_by(**{param: field.data}).first()
    if (obj is not None and (not hasattr(form, "assoc_obj") or obj != form.assoc_obj)) == unique:
      raise ValidationError(message or ("This %s is already in use!" if unique else "This %s does not exist!") % (name or param))
  return _inner

def email_format(form, field):
  if not validate_email.validate_email(field.data):
    raise ValidationError(ERROR_MESSAGES["invalid_email"])

def empty(form, field):
  if field.data:
    raise ValidationError("Input must be empty!")

id_charset = string.ascii_letters + string.digits + "-"
def id_format(form, field):
  if any(char not in id_charset for char in field.data):
    raise ValidationError(ERROR_MESSAGES["id_characters_invalid"])

def length(min_length = 0, max_length = float("inf"), name = "Input", end_message = ""):
  if min_length > 0 and max_length < float("inf"):
    message = "{name} must be between {min_length} and {max_length} characters long!"
  if min_length <= 0 and max_length < float("inf"):
    message = "{name} can be {max_length} characters long at most!"
  if min_length > 0 and max_length >= float("inf"):
    message = "{name} must be at least {min_length} characters long!"
  if min_length <= 0 and max_length >= float("inf"):
    message = "" # This check won't ever fail anyway
  if end_message:
    end_message = " " + end_message
  def _inner(form, field):
    if len(field.data) < min_length or len(field.data) > max_length:
      raise ValidationError(message.format(name = name, min_length = min_length, max_length = max_length) + end_message)
  return _inner

def no_controls(form, field):
  if any(unicodedata.category(char)[0] == "C" for char in field.data):
    raise ValidationError("No control characters allowed! This includes tabs and newlines (if you copy-pasted from another source)")

def OR(*validators, message = None):
  def _inner(form, field):
    errors = []
    for v in validators:
      try:
        v(form, field)
        return
      except ValidationError as e:
        errors.append(e)
    raise ValidationError(message or "One of the following must pass: " + "; ".join("[%s]" % error for error in errors))
  return _inner

def repeat_password(form, field):
  if field.data != form.password.data:
    raise ValidationError(ERROR_MESSAGES["password_mismatch"])

def require(message):
  return validators.DataRequired(message)

def timestamp_format(form, field):
  if field.data < -1:
    raise ValidationError("Please enter a valid timestamp (-1 for forever)!")

def validate_attendance(form, field):
  if AttendanceCodes.query.filter_by(oid = get_org_id(), code = field.data.strip()).count() == 0:
    raise ValidationError("Invalid attendance code!")

def validate_organization_join_code(form, field):
  organization = Organizations.query.filter_by(id = get_org_id()).first()
  if organization.can_join_code:
    if field.data != organization.join_code:
      raise ValidationError("Invalid join code!")
  else:
    raise ValidationError("This organization is not currently allowing Join By Code!")

def verify_login(param):
  def _inner(form, field):
    user = Users.query.filter_by(**{param: getattr(form, param).data}).first()
    if user and user.password_hash:
      if pass_hash(field.data, user.salt) == user.password_hash:
        if get_time() >= user.permissions.can_login_after:
          return
        else:
          raise ValidationError(ERROR_MESSAGES["account_disabled"])
    raise ValidationError(ERROR_MESSAGES["invalid_credentials"])
  return _inner
