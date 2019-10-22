from os import urandom
from base64 import b64encode

def _test_change_pass(client, user, num, assert_redirect):
  newpass = b64encode(urandom(16))
  oldpass = 'testpass%d' % num
  username = "Test User %d" % num
    
  assert_redirect(user.post("/login/?reauth=yes", data = dict(
    username = username,
    password = oldpass
  )), route = "/")
  
  assert_redirect(user.post("/change-password/", data = dict(
    password = newpass,
    repeat_password = newpass
  )), route = "/edit-profile/")
  
  assert client.post("/login/", data = dict(
    username = username,
    password = oldpass
  )).status_code == 400
  
  assert_redirect(client.post("/login/", data = dict(
    username = username,
    password = newpass
  )), route = "/")
  
  assert_redirect(client.get("/logout/"), route = "/")
  
  # Reset the password to original to ensure clean tests
  assert_redirect(user.post("/change-password/", data = dict(
    password = oldpass,
    repeat_password = oldpass
  )), route = "/edit-profile/")
  
def test_change_password(client, user1, user2, user3, assert_redirect):
  _test_change_pass(client, user1, 1, assert_redirect)
  _test_change_pass(client, user2, 2, assert_redirect)
  _test_change_pass(client, user3, 3, assert_redirect)