# -*- coding: utf-8 -*-

import string, uuid

# Encode an integer such that it only contains letters and numbers
def encode(i):
  chars = string.ascii_letters + string.digits
  res = ""
  power = 1
  while power <= i:
    res += chars[i // power % 62]
    power *= 62
  return res
  
def new_url_uuid():
  # Return a url_uuid, a 22 character string containing only letters and number
  return encode(uuid.uuid4().int)