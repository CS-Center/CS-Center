# -*- coding: utf-8 -*-

from wcics.database.consts import ATTENDANCE_CODE_MAX_LENGTH
from wcics.database.consts import NEWS_BODY_MAX_LENGTH, NEWS_ID_MAX_LENGTH, NEWS_TITLE_MAX_LENGTH
from wcics.database.consts import USER_DESC_MAX_LENGTH, USER_EMAIL_MAX_LENGTH, USER_PASSWORD_MAX_LENGTH, USER_REAL_NAME_MAX_LENGTH, USER_USERNAME_MAX_LENGTH, USER_PASSWORD_MIN_LENGTH

ERROR_MESSAGES = {
  "account_disabled": "This account is disabled! See the <a href='/faq#account-disabled'>FAQ</a> for more information.",
  "attendance_characters_invalid": "Attendance codes can only contain basic latin letters, digits, and dashes!",
  "attendance_code_too_long": "Attendance codes can be %d characters at most!" % ATTENDANCE_CODE_MAX_LENGTH,
  "attendance_invalid": "The given attendance code is wrong; please check your spelling (it is case-sensitive)!",
  "attendance_repeat": "This attendance code has already been used!",
  "authors_format_invalid": "The format for the authors field was incorrect; if you submitted it manually, please make sure it is a space-separated list of integers; otherwise, please <a href='/contact/'>contact us</a>!",
  "captcha_unsolved": "Please complete the reCAPTCHA challenge!",
  "creation_token_expired": "The account creation token was expired; please sign up again!",
  "creation_token_invalid": "The account creation token was invalid; if this issue was encountered from clicking a link in an email from our site, please <a href='/contact'>contact us</a>!",
  "edit_user_gone": "This account somehow stopped existing between when you clicked the button and when this is registering; please <a href='/contact'>contact us</a> if this issue remains upon retrying!",
  "email_exists": "An account with this email address already exists!",
  "email_not_string": "Somehow, the email wasn't a string. This should never happen; please <a href='/contact'>contact us</a> if this issue persists!",
  "email_too_long": "Email addresses can only be %d characters long at most!" % USER_EMAIL_MAX_LENGTH,
  "id_characters_invalid": "IDs can only contain basic latin letters, digits, and dashes!",
  "invalid_credentials": "Invalid credentials!",
  "invalid_email": "This email address is not in a recognized valid format; please enter a valid email address!",
  "invalid_password": "Incorrect password!",
  "invalid_username": "Control characters are not allowed in the username; please re-type your username!",
  "legal_agreement": "You must agree to our Terms of Service and Privacy Policy in order to access our site!",
  "mail_error": "An unknown mailing error occurred; please try again and <a href='/contact'>contact us</a> if this issue persists!",
  "news_body_too_long": "News body can only be %d characters at most!" % NEWS_BODY_MAX_LENGTH,
  "news_id_exists": "This ID is already in use!",
  "news_id_too_long": "News ID can only be %d characters at most!" % NEWS_ID_MAX_LENGTH,
  "news_title_too_long": "News titles can be %d characters at most!" % NEWS_TITLE_MAX_LENGTH,
  "no_such_user_email": "There is no account associated with this email address!",
  "no_such_user_username": "There is no account with this username!",
  "password_mismatch": "Passwords don't match!",
  "password_not_string": "The password was not a string. If you used the form, please <a href='/contact'>contact us</a> as this should never happen!",
  "password_too_long": "Passwords can be %d characters at most!" % USER_PASSWORD_MAX_LENGTH,
  "password_too_short": "Passwords must be at least %d characters long!" % USER_PASSWORD_MIN_LENGTH,
  "profile_desc_too_long": "Profile descriptions can be %d characters at most!" % USER_DESC_MAX_LENGTH,
  "real_name_too_long": "The name you entered as your real name can be %d characters at most; if you believe this is a mistake, please <a href='/contact'>contact us</a>!" % USER_REAL_NAME_MAX_LENGTH,
  "reset_token_expired": "The password reset token was expired; please try again!",
  "reset_token_invalid": "The password reset token was invalid; if this issue was encountered from clicking a link in an email from our site, please <a href='/contact'>contact us</a>!",
  "reset_token_mismatch": "The ID and email in the password reset token did not match; if the account email was changed, this is expected; otherwise, please <a href='/contact'>contact us</a>!",
  "subscribed_not_bool": "The `receive notification emails` value was not a true/false value. If you used the form, please <a href='/contact'>contact us</a> as this should never happen!",
  "username_exists": "This username is already taken!",
  "username_not_string": "The username was not a string. If you used the form, please <a href='/contact'>contact us</a> as this should never happen!",
  "username_too_long": "Usernames can be %d characters at most!" % USER_USERNAME_MAX_LENGTH
}
