# -*- coding: utf-8 -*-

from ..aliases import *
from ..helper import Helper

class organization_users(dbmodel, Helper):
  uid = dbcol(dbint, dbforkey('users.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  oid = dbcol(dbint, dbforkey('organizations.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  
  def __repr__(self):
    return "<organization_user uid=%d oid=%d>" % (self.uid, self.oid)

class organization_invites(dbmodel, Helper):
  uid = dbcol(dbint, dbforkey('users.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  oid = dbcol(dbint, dbforkey('organizations.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  
  def __repr__(self):
    return "<organization_invite uid=%d oid=%d>" % (self.uid, self.oid)

OrganizationUsers = organization_users
OrganizationInvites = organization_invites