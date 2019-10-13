# Test basic authentication; say login and logoff

def test_login_off(user1, user2, user3):
  assert user1.get("/logout/?next=/").status_code == 303
  assert user1.post("/login", data = dict(
    username = "Test User 1",
    password = "testpass1"
  ))