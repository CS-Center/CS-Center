# -*- coding: utf-8 -*-

from ..consts.users import USER_EMAIL_MAX_LENGTH, USER_REAL_NAME_MAX_LENGTH, USER_USERNAME_MAX_LENGTH, USER_DESC_MAX_LENGTH
from ..utils import db_commit
from .aliases import *
from .helper import Helper
from .attendance import AttendanceCodes, AttendanceRecords
from .news import News, NewsAuthors
from .organizations import Organizations, OrganizationUsers
from .permissions import Permissions
from .roles import OrganizationRoles, Roles

from wcics.utils.time import get_time
from wcics.utils.url import get_org_id

# User table
# Stores all of the information we could possibly want to know about a user
class users(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  username = dbcol(dbstr(USER_USERNAME_MAX_LENGTH), unique = True, nullable = False)
  email = dbcol(dbstr(USER_EMAIL_MAX_LENGTH), unique = True, nullable = False)
  real_name = dbcol(dbstr(USER_REAL_NAME_MAX_LENGTH), default = "", nullable = False)
  desc = dbcol(dbstr(USER_DESC_MAX_LENGTH), default = "", nullable = False)
  subscribed = dbcol(dbbool, default = True, nullable = False)
  # Admin is responsible for everything
  admin = dbcol(dbbool, default = False, nullable = False)
  join_time = dbcol(dbint, default = get_time, nullable = False)
  salt = dbcol(dbbinary, nullable = False)
  # Password is null if the user is using OAuth only
  password_hash = dbcol(dbbinary, nullable = True)

  # After the user has been created, use this to create permissions and roles
  def create_dependencies(self, commit = True):
    Permissions.add(uid = self.id)
    Roles.add(uid = self.id)
    Organizations.query.filter_by(oid = "main").first().add_user(self)

    if commit:
      db_commit()

  def __repr__(self):
    return "<User username=%s>" % self.username

  # Relationships
  roles = dbrelate(Roles, uselist = False, backref = 'user', cascade = 'all, delete-orphan')
  permissions = dbrelate(Permissions, uselist = False, backref = 'user', cascade = 'all, delete-orphan')
  
  def attendance_organizations(self):
    time = get_time()
    return Organizations.query.\
      join(OrganizationUsers).\
      join(Users).\
      join(AttendanceCodes).\
      filter(
        ~db.exists().\
          where(db.and_(AttendanceRecords.uid == Users.id, AttendanceRecords.cid == AttendanceCodes.id)), 
        Users.id == self.id, AttendanceCodes.start <= time, time <= AttendanceCodes.end
      ).distinct(Organizations.id).all()
  
  def news_admin_organizations(self):
    subq = db.exists().where(db.and_(NewsAuthors.uid == self.id, News.oid == Organizations.id))
    realq = Organizations.query.\
      join(OrganizationUsers).\
      join(OrganizationRoles).\
      filter(OrganizationRoles.uid == self.id, db.or_(OrganizationRoles.news >= 1, subq))
      
    return realq.all()
  
  def lesson_admin_organizations(self):
    subq = db.exists().where(db.and_(LessonAuthors.uid == self.id, News.oid == Organizations.id))
    realq = Organizations.query.\
      join(OrganizationUsers).\
      join(OrganizationRoles).\
      filter(OrganizationRoles.uid == self.id, db.or_(OrganizationRoles.lessons >= 1, subq))
    return realq.all()
  
  @property
  def organizations(self):
    return Organizations.query.join(OrganizationUsers).filter(OrganizationUsers.uid == self.id).all()
  
  @property
  def organization_roles(self):
    return OrganizationRoles.query.filter_by(uid = self.id, oid = get_org_id()).first()

# This allows easy renaming of tables in our code
# NEVER IMPORT users, always import Users
Users = users
