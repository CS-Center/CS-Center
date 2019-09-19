import pytest

from wcics import app

@pytest.fixture(scope = "session")
def client():
  return app.test_client()