# -*- coding: utf-8 -*-

from ...consts.organizations import ORGANIZATION_ID_MAX_LENGTH, ORGANIZATION_JOIN_CODE_MAX_LENGTH, ORGANIZATION_LOGO_URL_MAX_LENGTH, ORGANIZATION_NAME_MAX_LENGTH, ORGANIZATION_DESC_MAX_LENGTH
from ...utils import db_commit

from ..aliases import *
from ..helper import Helper
from ..roles import OrganizationRoles

from .users import OrganizationUsers

from wcics.utils.time import get_time

class organizations(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  oid = dbcol(dbstr(ORGANIZATION_ID_MAX_LENGTH), unique = True, nullable = False)
  name = dbcol(dbstr(ORGANIZATION_NAME_MAX_LENGTH), nullable = False)
  desc = dbcol(dbstr(ORGANIZATION_DESC_MAX_LENGTH), nullable = False)
  create_time = dbcol(dbint, nullable = False, default = get_time)
  hidden = dbcol(dbbool, nullable = False, default = False)
  logo = dbcol(dbstr(ORGANIZATION_LOGO_URL_MAX_LENGTH), nullable = False, default = "")
  
  join_code = dbcol(dbstr(ORGANIZATION_JOIN_CODE_MAX_LENGTH), nullable = False, default = "")
  can_join_code = dbcol(dbbool, nullable = False, default = False)
  can_apply = dbcol(dbbool, nullable = False, default = False)
  
  def __repr__(self):
    return "<organization oid=%s>" % (self.oid)
  
  def add_user(self, user):
    # THiS IS IMPORTANT, IF NOT IT VALIDATES A FOREIGN KEY CONSTRAINT FOR ORGAQNIZATION_ROLES
    OrganizationUsers.add(oid = self.id, uid = user.id)
    db_commit()
    
    OrganizationRoles.add(oid = self.id, uid = user.id)
    db_commit()
    return self
  
  def has_user(self, user):
    return OrganizationUsers.query.filter_by(oid = self.id, uid = user.id).count() >= 1
  
  def user_role(self, user):
    return OrganizationRoles.query.filter_by(oid = self.id, uid = user.id).first().admin
  
Organizations = organizations