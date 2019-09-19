# -*- coding: utf-8 -*-

from wcics import logger

from wcics.auth.manage_user import user

from wcics.database.models import Organizations, OrganizationUsers
from wcics.database.models.roles import AttendanceRoles, DatabaseRoles, NewsRoles, OrganizationRoles, TopicRoles, UserRoles

def admin_sublinks():
  links = []
  if user.roles.users > UserRoles.default:
    links.append(("/admin/users/", "users", []))
  if user.roles.database > DatabaseRoles.default:
    links.append(("/admin/database/", "database", []))
  if False: # TODO
    links.append(("/admin/news/", "news", []))
  if any(role.attendance > AttendanceRoles.default for role in OrganizationRoles.query.filter_by(uid = user.id).all()):
    links.append(("/admin/attendance/", "attendance", [("/organization/%s/admin/attendance/" % organization.oid, organization.name) for organization in Organizations.query.all() if organization.id != 1 and OrganizationUsers.query.filter_by(oid = organization.id, uid = user.id).count() > 0 and OrganizationRoles.query.filter_by(oid = organization.id, uid = user.id).first().attendance > AttendanceRoles.default]))
  if False:
    links.append(("/admin/topics/", "topics", []))
  if False: # TODO
    links.append(("/admin/learn/", "learn", []))
  if False: # TODO
    links.append(("/admin/problems/", "problems", []))
  if False: # TODO
    links.append(("/admin/editorials/", "editorials", []))
  return links

def links():
  links = [
    ("/", "Home", []),
#     ("/problems/", "Problems", []),
#     ("/learn/", "Learn", []),
    ("/calendar/", "Calendar", []),
    ("/users/", "Users", []),
    ("/organizations/", "Organizations", []),
#     ("/editorials/", "Editorials", []),
    ("/about/", "About", [("/contact", "Contact"), ("/faq", "FAQ")])
  ]
  if user and (
    user.roles.database   >     DatabaseRoles.default or
    user.roles.users      >         UserRoles.default
  ):
    links.insert(1, ("/admin/", "admin", [(x, y) for x, y, z in admin_sublinks()]))

  return links

def sudo_links():
  links = [
    ("/", "Home", []),
    ("/admin/", "admin", [])
  ]
  if not user:
    logger.warn("User got sudo_links without being signed in [???]")
    return links # This shouldn't happen
  return links + admin_sublinks()