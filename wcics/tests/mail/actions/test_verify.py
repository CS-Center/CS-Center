# -*- coding: utf-8 -*-

import pytest

from wcics import app, mail_app

from wcics.mail.actions.verify import *

from wcics.tests.mail.consts import test_addrs

test_next_url = '/about/'

test_token = 'asdgyauu'

@pytest.mark.parametrize('addr', test_addrs)
def test_send_verify(addr):
  # Push app context so mail can be sent
  with app.app_context():
    # Record messages
    with mail_app.record_messages() as outbox:
      send_verify_email(addr, test_token, test_next_url).join()
      
      msg = outbox[0]
      
      assert msg.recipients[0] == addr
      assert msg.subject == verify_subject
      assert msg.html == verify_body.format(
        next = test_next_url, 
        jwt = test_token,
        DOMAIN = DOMAIN
      )