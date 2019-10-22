# -*- coding: utf-8 -*-

from .wci_validators import *

from wcics.auth.manage_user import user

from wcics.database.consts import *
from wcics.database.models import *

#############
## ACCOUNT ##
#############

# LOGIN #

# EMAIL
account_login_email_email = [email_format, require("Please enter your email!")]
account_login_email_password = [verify_login("email"), require("Please enter your password!")]

# USERNAME
account_login_username_username = [require("Please enter your username!")]
account_login_username_password = [verify_login("username"), require("Please enter your password!")]

# SIGNUP #

# CREATE
account_signup_create_email = []
account_signup_create_username = [no_controls, check_unique(Users, "username"), require("Please enter a username!"), length(max_length = USER_USERNAME_MAX_LENGTH, name = "Username")]
account_signup_create_real_name = [no_controls, length(max_length = USER_REAL_NAME_MAX_LENGTH, name = "Real name", end_message= "Please <a href='/contact'>contact us</a> if you believe this is a mistake.")]
account_signup_create_password = [require("Please enter a password!"), length(max_length = USER_PASSWORD_MAX_LENGTH, name = "Password")]
account_signup_create_repeat_password = [repeat_password, require("Please re-enter your password to make sure you don't accidentally mistype it!")]
account_signup_create_legal_agreement = [assert_checked("You must agree to the terms and conditions and privacy policy to use our site!")]
account_signup_create_subscribed = []

# REQUEST
account_signup_request_email = [email_format, length(max_length = USER_EMAIL_MAX_LENGTH, name = "Email"), require("Please enter your email!")]

# #

# ATTENDANCE
account_attendance_attendance_code = [validate_attendance, require("Please enter the attendance code!")]

# CHANGE PASSWORD
account_change_password_password = [require("Please enter a password!"), length(max_length = USER_PASSWORD_MAX_LENGTH, name = "Password")]
account_change_password_repeat_password = [repeat_password, require("Please re-enter your password to make sure you don't accidentally mistype it!")]

# EDIT PROFILE
account_edit_profile_description = [length(max_length = USER_DESC_MAX_LENGTH, name = "Profile descrption")]
account_edit_profile_username = [no_controls, length(max_length = USER_USERNAME_MAX_LENGTH, name = "Username"), check_unique(Users, "username"), require("Please enter a username!")]
account_edit_profile_real_name = [no_controls, length(max_length = USER_REAL_NAME_MAX_LENGTH, name = "Real name", end_message = "Please <a href='/contact'>contact us</a> if you believe this is a mistake.")]
account_edit_profile_email = [email_format, length(max_length = USER_EMAIL_MAX_LENGTH, name = "Email"), check_unique(Users, "email"), require("Please enter your email!")]
account_edit_profile_subscribed = []

# RESET PASSWORD
account_reset_password_email = [email_format, length(max_length = USER_EMAIL_MAX_LENGTH, name = "Email"), require("Please enter your email!")]

###########
## ADMIN ##
###########

# NEWS #

# CREATE
admin_news_create_nid = [id_format, length(max_length = NEWS_ID_MAX_LENGTH, name = "News ID"), require("Please enter an ID!"), check_unique(News, "nid", name = "ID")]
admin_news_create_title = [no_controls, length(max_length = NEWS_TITLE_MAX_LENGTH, name = "News title"), require("Please enter a title!")]
admin_news_create_body = [length(max_length = NEWS_BODY_MAX_LENGTH, name = "News body"), require("Please enter a body!")]
admin_news_create_authors = [author_format]
admin_news_create_email = []

# EDIT
admin_news_edit_nid = [id_format, length(max_length = NEWS_ID_MAX_LENGTH, name = "News ID"), require("Please enter an ID!"), check_unique(News, "nid", name = "ID")]
admin_news_edit_title = [no_controls, length(max_length = NEWS_TITLE_MAX_LENGTH, name = "News title"), require("Please enter a title!")]
admin_news_edit_body = [length(max_length = NEWS_BODY_MAX_LENGTH, name = "News body"), require("Please enter a body!")]
admin_news_edit_authors = [author_format]
admin_news_edit_delete = []

# LESSONS #

# CREATE
admin_lesson_create_nid = [id_format, length(max_length = LESSON_ID_MAX_LENGTH, name = "Lesson ID"), require("Please enter an ID!"), check_unique(Lessons, "lid", name = "ID")]
admin_lesson_create_title = [no_controls, length(max_length = LESSON_TITLE_MAX_LENGTH, name = "Lesson title"), require("Please enter a title!")]
admin_lesson_create_desc = [length(max_length = LESSON_DESC_MAX_LENGTH, name = "Lesson description"), require("Please enter a description!")]
admin_lesson_create_body = [length(max_length = LESSON_BODY_MAX_LENGTH, name = "Lesson body"), require("Please enter a body!")]
admin_lesson_create_authors = [author_format]

# EDIT
admin_lesson_edit_nid = [id_format, length(max_length = LESSON_ID_MAX_LENGTH, name = "Lesson ID"), require("Please enter an ID!"), check_unique(Lessons, "lid", name = "ID")]
admin_lesson_edit_title = [no_controls, length(max_length = LESSON_TITLE_MAX_LENGTH, name = "Lesson title"), require("Please enter a title!")]
admin_lesson_edit_desc = [length(max_length = LESSON_DESC_MAX_LENGTH, name = "Lesson description"), require("Please enter a description!")]
admin_lesson_edit_body = [length(max_length = LESSON_BODY_MAX_LENGTH, name = "Lesson body"), require("Please enter a body!")]
admin_lesson_edit_authors = [author_format]
admin_lesson_edit_delete = []

# TOPICS #

# CREATE
admin_topic_create_tid = [id_format, length(max_length = TOPIC_ID_MAX_LENGTH, name = "Topic ID"), require("Please enter an ID!"), check_unique(Topics, "tid", name = "ID")]
admin_topic_create_name = [no_controls, length(max_length = TOPIC_NAME_MAX_LENGTH, name = "Topic name"), require("Please enter a name!")]
admin_topic_create_description = [length(max_length = TOPIC_DESC_MAX_LENGTH, name = "Topic description")]

# EDIT
admin_topic_edit_tid = [id_format, length(max_length = TOPIC_ID_MAX_LENGTH, name = "Topic ID"), require("Please enter an ID!"), check_unique(Topics, "tid", name = "ID")]
admin_topic_edit_name = [no_controls, length(max_length = TOPIC_NAME_MAX_LENGTH, name = "Topic name"), require("Please enter a name!")]
admin_topic_edit_description = [length(max_length = TOPIC_DESC_MAX_LENGTH, name = "Topic description")]

# USERS #

# ADMIN
admin_users_admin_role_users = []
admin_users_admin_role_news = []
admin_users_admin_role_database = []
admin_users_admin_role_attendance = []
admin_users_admin_role_topics = []

# MODERATOR
admin_users_moderator_can_login_after = [timestamp_format]

# #

# ATTENDANCE
admin_attendance_attendance_code = [attendance_code_format, length(max_length = ATTENDANCE_CODE_MAX_LENGTH, name = "Attendance code")]

# DATABASE
admin_database_query = []

# ORGANIZATION
admin_organization_name = [no_controls, length(max_length = ORGANIZATION_NAME_MAX_LENGTH, name = "Organization name"), require("Please enter a name!")]
admin_organization_description = [length(max_length = ORGANIZATION_DESC_MAX_LENGTH, name = "Organization description")]
admin_organization_join_code = [no_controls, length(max_length = ORGANIZATION_JOIN_CODE_MAX_LENGTH, name = "Organization join code")]
admin_organization_can_join_code = []
admin_organization_can_apply = []

# ORGANIZATIONS #

# JOIN BY CODE
organizations_join_by_code_join_code = [validate_organization_join_code]