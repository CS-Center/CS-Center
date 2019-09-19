# -*- coding: utf-8 -*-

# Mailing utilities

from wcics import mail_app

# Send a message to a single person
def send_single(addr, subject = "", body = ""):
  mail_app.send_message(subject, recipients = [addr], html = body)
    
# Send a message to many people
def send_many(addrs, subject = "", body = ""):
  with mail_app.connect() as conn:
    for addr in addrs:
      conn.send_message(subject, recipients = [addr], html = body)