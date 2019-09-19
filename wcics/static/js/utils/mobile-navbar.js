$(document).ready(() => {
  $("#navbar-expand").css("top", $("#navbar").height()).hide();
  
  $("#toggle-navbar").click(event => {
    $("#navbar-expand").toggle().focus();
  });
  $("#navbar-expand").blur(event => {
    if (!$("#toggle-navbar").is(":active") && $(".navbar__row__button:active").length == 0) {
      $("#navbar-expand").hide();
    }
  });
});