# Test the main routes of site
# All are extremely basic, no auth

import pytest

from .consts import MAIN_ROUTES

@pytest.mark.parametrize("route", MAIN_ROUTES)
def test_main_route(client, route):
  assert client.get(route).status_code == 200