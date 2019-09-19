# -*- coding: utf-8 -*-

# Thrown when an action token's payload is ill-formatted
class InvalidActionPayload(Exception):
  def __init__(self, payload):
    self.payload = payload
    
# Thrown when the action being processed was sent to an email that is no longer the same as the user's current email
class ActionEmailChange(Exception):
  def __init__(self, old_email, new_email):
    self.old = old_email
    self.new = new_email
    
# Thrown when the action type does not exist
class InvalidActionType(Exception):
  def __init__(self, type):
    self.type = type