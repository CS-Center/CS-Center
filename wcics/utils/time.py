# -*- coding: utf-8 -*-

import calendar, datetime, time

def get_time():
  return int(time.time())

def time_within(*units):
  return False if len(units) == 0 else units[0] < 0 or units[0] == 0 and time_within(*units[1:])

def time_delta_display(dt):
  ct = datetime.datetime.now()
  dt = datetime.datetime.fromtimestamp(dt)
  
  s = ct.second - dt.second
  m = ct.minute - dt.minute
  h = ct.hour - dt.hour
  d = ct.day - dt.day
  n = ct.month - dt.month
  y = ct.year - dt.year
  
  S, M, H, D, N, Y = s, m, h, d, n, y
  
  if time_within(s):
    M -= 1
    S += 60
  
  if time_within(m, s):
    H -= 1
    M += 60
    
  if time_within(h, m, s):
    D -= 1
    H += 24
  
  if time_within(d, h, m, s):
    N -= 1
    D += [0, 31, 29 if calendar.isleap(dt.year) else 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31][dt.month]
  
  if time_within(n, d, h, m, s):
    Y -= 1
    M += 12
  
  for val, name in [(Y, "year"), (N, "month"), (D, "day"), (H, "hour"), (M, "minute"), (S, "second")]:
    if val == 1:
      return "a%s %s ago" % ("n" if name == "hour" else "", name)
    elif val > 0:
      return "%d %ss ago" % (val, name)
  
  return "just now"