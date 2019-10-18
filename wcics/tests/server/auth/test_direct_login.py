# Test direct-login

from wcics.auth.jwt import make_jwt

from wcics.utils.time import get_time

import pytest

@pytest.mark.parametrize("uid", [1, 2, 3])
def test_direct_login(uid, client, assert_redirect):
  assert_redirect(client.get("/direct-login/?token=%s" % make_jwt(dict(
    exp = get_time() + 60,
    email = "test%d@gmail.com" % uid
  ))), route = "/")
  
  assert_redirect(client.get("/logout/"), route = "/")
  
def test_direct_login_invalid(client):  
  assert client.get("/direct-login/?token=a").status_code == 400
    
def test_direct_login_expired(client):
  assert client.get("/direct-login/?token=%s" % make_jwt(dict(
    exp = get_time() - 1,
    email = "test1@gmail.com"
  ))).status_code == 400

def test_direct_login_auth_redir(user1, user2, user3, assert_redirect):
  assert_redirect(user1.get("/direct-login/"), route = "/")
  assert_redirect(user2.get("/direct-login/"), route = "/")
  assert_redirect(user3.get("/direct-login/"), route = "/")