# -*- coding: utf-8 -*-

class MailError(Exception):
  def __init__(self, addrs):
    self.failed = addrs