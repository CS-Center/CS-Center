# -*- coding: utf-8 -*-

from .aliases import *
from .helper import Helper

class permissions(dbmodel, Helper):
  uid = dbcol(dbint, dbforkey('users.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)

  # A time stamp for the time until they are allowed to login
  # Default means they can always login
  can_login_after = dbcol(dbint, default = 0, nullable = False)
  
  # Revoke tokens minted before this stamp
  revoke_tokens_before = dbcol(dbint, default = 0, nullable = False)
  
  def __repr__(self):
    return "<Permissions for %s>" % self.user.username
  
# Alias
Permissions = permissions