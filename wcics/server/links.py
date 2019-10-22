# -*- coding: utf-8 -*-

from wcics import logger

from wcics.auth.manage_user import user

from wcics.database.models import Organizations, OrganizationUsers
from wcics.database.models.roles import AttendanceRoles, DatabaseRoles, NewsRoles, OrganizationRoles, TopicRoles, UserRoles

from wcics.utils.url import get_org_id

def admin_sublinks():
  if not user: return []
  links = []
  if user.roles.users > UserRoles.default:
    links.append(("/admin/users/", "users", []))
  if user.roles.database > DatabaseRoles.default:
    links.append(("/admin/database/", "database", []))
  if user.news_admin_organizations():
    links.append(("/admin/news/", "news", [("/organization/%s/admin/news/" % organization.oid, organization.name) for organization in user.news_admin_organizations()]))
  if any(role.attendance > AttendanceRoles.default for role in OrganizationRoles.query.filter_by(uid = user.id).all()):
    links.append(("/admin/attendance/", "attendance", [("/organization/%s/admin/attendance/" % organization.oid, organization.name) for organization in Organizations.query.all() if organization.id != 1 and OrganizationUsers.query.filter_by(oid = organization.id, uid = user.id).count() > 0 and OrganizationRoles.query.filter_by(oid = organization.id, uid = user.id).first().attendance > AttendanceRoles.default]))
  if False:
    links.append(("/admin/topics/", "topics", []))
  if user.lesson_admin_organizations(): # TODO
    links.append(("/admin/lessons/", "lessons", []))
  if False: # TODO
    links.append(("/admin/problems/", "problems", []))
  if False: # TODO
    links.append(("/admin/editorials/", "editorials", []))
  return links

def links():
  co = Organizations.query.filter_by(id = get_org_id()).first()
  links = [
    ("/" if co.id == 1 else "/organization/%s" % co.oid, "CSCenter Home" if co.id == 1 else "%s Home" % co.name, [("/", "CSCenter Home") if org.id == 1 else ("/organization/%s" % org.oid, org.name + " Home") for org in Organizations.query.join(OrganizationUsers).filter(OrganizationUsers.uid == user.id, Organizations.id == OrganizationUsers.oid, Organizations.id != co.id).all()] if user else []),
#     ("/problems/", "Problems", []),
    ("/lessons/" if co.id == 1 else "/organization/%s/lessons/" % co.oid, "Lessons", []),
    ("/calendar/", "Calendar", []),
    ("/users/" if co.id == 1 else "/organization/%s/users/" % co.oid, "Users", []),
    ("/organizations/", "Organizations", []),
#     ("/editorials/", "Editorials", []),
    ("/about/", "About", [("/contact", "Contact"), ("/faq", "FAQ")])
  ]
  sub = admin_sublinks()
  if sub:
    links.insert(1, ("/admin/", "admin", [(x, y) for x, y, z in sub]))

  return links

def sudo_links():
  co = Organizations.query.filter_by(id = get_org_id()).first()
  links = [
    ("/" if co.id == 1 else "/organization/%s" % co.oid, "CSCenter Home" if co.id == 1 else "%s Home" % co.name, [("/", "CSCenter Home") if org.id == 1 else ("/organization/%s" % org.oid, org.name + " Home") for org in Organizations.query.join(OrganizationUsers).filter(OrganizationUsers.uid == user.id, Organizations.id == OrganizationUsers.oid, Organizations.id != co.id).all()] if user else []),
    ("/admin/", "admin", [])
  ]
  if not user:
    logger.warn("User got sudo_links without being signed in [???]")
    return links # This shouldn't happen
  return links + admin_sublinks()