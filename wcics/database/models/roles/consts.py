# -*- coding: utf-8 -*-

class AttendanceRoles:
  default = 0
  reader = 1
  admin = 2

class DatabaseRoles:
  default = 0
  admin = 1

class NewsRoles:
  underprivileged = -1
  default = 0
  poster = 1
  moderator = 2

# This class is for creating/destroying organizations as the site admin
class OrganizationAdminRoles:
  default = 0
  admin = 1

# This class is for managing organizations as the organization owner
class OrganizationManagerRoles:
  default = 0
  inviter = 1
  admin = 2

class TopicRoles:
  default = 0
  editor = 1
  admin = 2

class UserRoles:
  default = 0
  moderator = 1
  admin = 2

class LessonRoles:
  underprivileged = -1
  default = 0
  creator = 1
  admin = 2

class roles:
  database = DatabaseRoles
  news = NewsRoles
  users = UserRoles
  attendance = AttendanceRoles
  topics = TopicRoles

  org_admin = OrganizationAdminRoles
  org_manager = OrganizationManagerRoles

  lessons = LessonRoles
