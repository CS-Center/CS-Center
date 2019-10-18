import pytest

@pytest.mark.parametrize("uid", [1, 2, 3])
def test_user_route(uid, client, user1, user2, user3):
  route = "/user/%d" % uid
  
  assert client.get(route).status_code == 200
  assert user1.get(route).status_code == 200
  assert user2.get(route).status_code == 200
  assert user3.get(route).status_code == 200

def test_self_user_route(client, user1, user2, user3):
  assert client.get("/user/").status_code == 303
  assert user1.get("/user/").status_code == 200
  assert user2.get("/user/").status_code == 200
  assert user3.get("/user/").status_code == 200