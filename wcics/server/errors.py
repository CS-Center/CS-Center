# -*- coding: utf-8 -*-

class RedirectError(Exception):
  def __init__(self, response):
    self.response = response

class NotLoggedInError(Exception):
  pass

class SessionNotFreshError(Exception):
  pass