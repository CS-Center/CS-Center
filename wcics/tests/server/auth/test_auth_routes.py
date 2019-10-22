import pytest

from .consts import AUTH_ROUTES, AUTH_REDIR_ROUTES

@pytest.mark.parametrize("route", AUTH_ROUTES)
def test_auth_route(user1, user2, user3, route):
  assert user1.get(route).status_code == 200
  assert user2.get(route).status_code == 200
  assert user3.get(route).status_code == 200
  
@pytest.mark.parametrize("route", AUTH_REDIR_ROUTES)
def test_auth_redir(user1, user2, user3, route):
  assert user1.get(route).status_code == 303
  assert user2.get(route).status_code == 303
  assert user3.get(route).status_code == 303