import pytest, json

from wcics import app

from wcics.auth.jwt import make_jwt

from wcics.database.models import Users, Organizations, OrganizationRoles, Roles

from wcics.database.models.roles.consts import roles, OrganizationManagerRoles, OrganizationAdminRoles

from wcics.database.utils import db_commit

from werkzeug.urls import url_parse

@pytest.fixture(scope = "session")
def client():
  return app.test_client()

def make_test_user(num):
  tclient = app.test_client()
  
  email = "test%d@gmail.com" % num
  
  res = tclient.post('/signup/', data = dict(email = email))
    
  assert res.status_code == 303
  
  res = tclient.post("/create-account/?token=%s" % make_jwt(dict(email = email)), data = dict(
    username = "Test User %d" % num,
    real_name = "Test User %d" % num,
    password = "testpass%d" % num,
    repeat_password = "testpass%d" % num,
    legal_agreement = "y"
  ))
    
  assert res.status_code == 303
  
  return tclient

@pytest.fixture(scope = "session")
def user1():
  # User 1 has all the power
  cl = make_test_user(1)
  
  u = Users.query.filter_by(username = "Test User 1").first()
  
  o_main = Organizations.query.filter_by(oid = "main").first()
  o_test = Organizations.query.filter_by(oid = "test").first()
  
  o_test.add_user(u)
  
  main_roles = OrganizationRoles.query.filter_by(oid = o_main.id, uid = u.id).first()
  
  main_roles.admin = OrganizationManagerRoles.admin
  main_roles.attendance = roles.attendance.admin
  main_roles.news = roles.news.moderator
  
  test_roles = OrganizationRoles.query.filter_by(oid = o_test.id, uid = u.id).first()
  
  test_roles.admin = OrganizationManagerRoles.admin
  test_roles.attendance = roles.attendance.admin
  test_roles.news = roles.news.moderator
  
  general_roles = Roles.query.filter_by(uid = u.id).first()
  
  general_roles.database = roles.database.admin
  general_roles.organizations = OrganizationAdminRoles.admin
  general_roles.users = roles.users.admin
  
  db_commit()
  
  return cl

@pytest.fixture(scope = "session")
def user2():
  return make_test_user(2)
  
@pytest.fixture(scope = "session")
def user3():
  return make_test_user(3)

@pytest.fixture(scope = "session")
def assert_redirect():
  def inner(res, route, code = 303):
    
    assert res.status_code == code
    
    assert url_parse(res.headers['Location']).path == route
    
  return inner