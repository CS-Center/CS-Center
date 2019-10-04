# -*- coding: utf-8 -*-

from ..utils import create_tables

from .attendance import AttendanceRecords, AttendanceCodes
from .lessons import Lessons, LessonTopics, LessonAuthors
from .news import News, NewsAuthors
from .oauth import GoogleLinks, GithubLinks
from .organizations import Organizations, OrganizationUsers
from .permissions import Permissions
from .roles import Roles, OrganizationRoles
from .problems import Problems, ProblemSamples, ProblemSuites, ProblemCases
from .submissions import Submissions, SubmissionCases
from .topics import Topics
from .users import Users

# Initialize the tables
create_tables()
