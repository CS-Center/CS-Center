# -*- coding: utf-8 -*-

from ..aliases import *
from ..helper import Helper

class github_links(dbmodel, Helper):
  uid = dbcol(dbint, dbforkey("users.id"), primary_key = True)
  gid = dbcol(dbint, primary_key = True, unique = True)
  
  def __repr__(self):
    return "<github_link uid=%d gid=%d>" % (self.uid, self.gid)
  
GithubLinks = github_links