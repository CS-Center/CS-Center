# -*- coding: utf-8 -*-

from ..aliases import *
from ..helper import Helper

from .consts import AttendanceRoles, DatabaseRoles, OrganizationAdminRoles, NewsRoles, UserRoles

class roles(Helper, dbmodel):
  uid = dbcol(dbint, dbforkey('users.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  database = dbcol(dbsmallint, default = 0, nullable = False)
  organizations = dbcol(dbsmallint, default = 0, nullable = False)
  users = dbcol(dbsmallint, default = 0, nullable = False)
  
  def __repr__(self):
    return "<Roles for %s>" % self.user.username

class organization_roles(Helper, dbmodel):
  uid = dbcol(dbint, primary_key = True)
  oid = dbcol(dbint, primary_key = True)
  admin = dbcol(dbsmallint, default = 0, nullable = False)
  attendance = dbcol(dbsmallint, default = 0, nullable = False)
  
  __table_args__ = (db.ForeignKeyConstraint(["uid", "oid"], ["organization_users.uid", "organization_users.oid"], onupdate = "CASCADE", ondelete = "CASCADE"),)

Roles = roles
OrganizationRoles = organization_roles