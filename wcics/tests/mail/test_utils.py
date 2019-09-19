# -*- coding: utf-8 -*-

# Test mailing utils

import pytest

from wcics import app

from wcics.mail.utils import *

from wcics.tests.mail.consts import test_addrs

test_subject = 'Testing Email'

test_html = '<p>this is a test</p>'

@pytest.mark.parametrize('addr', test_addrs)
def test_send_single(addr):
  # Setup an app context for send_single to work
  with app.app_context():
    # Record messages
    with mail_app.record_messages() as outbox:
      send_single(addr, test_subject, test_html)

      assert len(outbox) == 1
      assert outbox[0].subject == test_subject
      assert outbox[0].html == test_html
      assert outbox[0].recipients[0] == addr

def test_send_many():
  # Setup an app context for send_single to work
  with app.app_context():
    # Record messages
    with mail_app.record_messages() as outbox:
      send_many(test_addrs, test_subject, test_html)

      assert len(outbox) == len(test_addrs)
      
      for i in range(len(outbox)):
        assert outbox[i].subject == test_subject
        assert outbox[i].html == test_html
        assert outbox[i].recipients[0] == test_addrs[i]