$(document).ready(() => {
  $("#global-logout").click(event => {
    if (confirm("Sign out of all locations?")) {
      POST("/global-logout", {
        "token": $("#csrf_token").val()
      }).then(() => window.location.reload());
    }
  });
});