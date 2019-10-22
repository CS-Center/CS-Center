from wcics.auth.jwt import make_jwt

from wcics.utils.time import get_time

def test_create_account_redir(user1):
  assert user1.get("/create-account/?token=%s" % make_jwt(dict(
    email = "test1@gmail.com",
    exp = get_time() + 60 
  ))).status_code == 303
  
def test_create_account_invalid(client):
  assert client.get("/create-account/?token=a").status_code == 400
  
def test_create_account_expired(client):
  assert client.get("/create-account/?token=%s" % make_jwt(dict(
    email = "test1@gmail.com",
    exp = get_time() - 1
  ))).status_code == 400
  
def test_create_account_valid(client):
  assert client.get("/create-account/?token=%s" % make_jwt(dict(
    email = "no-user@gmail.com",
    exp = get_time() + 60 
  ))).status_code == 200