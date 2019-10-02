# -*- coding: utf-8 -*-

NEWS_PER_PAGE = 10
ORGANIZATIONS_PER_PAGE = 50
USERS_PER_PAGE = 50

LESSONS_PER_PAGE = 10

PAGE_THRESHOLDS = {
  "edge_left": 2,
  "merge_left": 6,
  "show_left": 2,
  "edge_right": 2,
  "merge_right": 6,
  "show_right": 3
}

NO_REDIRECT = []

FLASHES = ["ERROR", "SUCCESS", "INFO", "DEBUG"] # TODO

ERRORS = {
  403: "Permission Denied",
  404: "Page Not Found",
  500: "Internal Server Error"
}

REPORT_MESSAGES = {
  403: "If you believe this is an error",
  404: "If you navigated to this URL via a link on this site"
}

DEFAULT_MESSAGES = {
  403: "You don't have permission. Not even if you say please.",
  404: "Whatever you were looking for, it's not here. We looked for it, we promise.",
  500: "This is an error on our part. We might be in the middle of some changes."
}