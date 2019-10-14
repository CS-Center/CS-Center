# Test basic authentication; say login and logoff

def test_login_off(user1, user2, user3):
  assert user1.get("/logout/").status_code == 303
  assert user1.post("/login/", data = dict(
    username = "Test User 1",
    password = "testpass1"
  )).status_code == 303
  
  assert user2.get("/logout/").status_code == 303
  assert user2.post("/login/", data = dict(
    username = "Test User 2",
    password = "testpass2"
  )).status_code == 303
  
  assert user3.get("/logout/").status_code == 303
  assert user3.post("/login/", data = dict(
    username = "Test User 3",
    password = "testpass3"
  )).status_code == 303
  
# login with email
def test_login_off_email(user1, user2, user3):
  assert user1.get("/logout/?next=/").status_code == 303
  assert user1.post("/login/?id=email", data = dict(
    email = "test1@gmail.com",
    password = "testpass1"
  )).status_code == 303
  
  assert user2.get("/logout/?next=/").status_code == 303
  assert user2.post("/login/?id=email", data = dict(
    email = "test1@gmail.com",
    password = "testpass1"
  )).status_code == 303
  
  assert user3.get("/logout/?next=/").status_code == 303
  assert user3.post("/login/?id=email", data = dict(
    email = "test1@gmail.com",
    password = "testpass1"
  )).status_code == 303