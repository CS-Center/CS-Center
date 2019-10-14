# Test direct-login

from wcics.auth.jwt import make_jwt

from wcics.utils.time import get_time

def test_direct_login(client):
  assert client.get("/direct-login/?token=%s" % make_jwt(dict(
    exp = get_time() + 60,
    email = "test1@gmail.com"
  ))).status_code == 303
  
  assert client.get("/logout/").status_code == 303
  
def test_direct_login_invalid(client):  
  assert client.get("/direct-login/?token=a").status_code == 400
    
def test_direct_login_expired(client):
  assert client.get("/direct-login/?token=%s" % make_jwt(dict(
    exp = get_time() - 1,
    email = "test1@gmail.com"
  ))).status_code == 400

def test_direct_login_auth_redir(user1, user2, user3):
  assert user1.get("/direct-login/").status_code == 303
  assert user2.get("/direct-login/").status_code == 303
  assert user3.get("/direct-login/").status_code == 303