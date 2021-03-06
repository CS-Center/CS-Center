# -*- coding: utf-8 -*-

# Mailing utilities

import threading, traceback

from wcics import mail_app, app

# Send a message to a single person
# return the thread
def send_single(addr, subject = "", body = ""):
  def inner():    
    # You need an app context for this :/
    with app.app_context():
      mail_app.send_message(subject, recipients = [addr], html = body)
    
  t = threading.Thread(target = inner)
  
  t.start()
  
  return t
  
# Send a message to many people
def send_many(addrs, subject = "", body = ""):
  def inner():
    with app.app_context():
      with mail_app.connect() as conn:
        for addr in addrs:
          try:
            conn.send_message(subject, recipients = [addr], html = body)
          except:
            traceback.print_exc()
  
  t = threading.Thread(target = inner)
  
  t.start()
  
  return t