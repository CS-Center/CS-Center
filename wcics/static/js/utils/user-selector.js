$(document).ready(() => {
  $("#username-matches").children().hide();
  
  $("#target").on("input", event => {
    var query = $("#target").val().toLowerCase();
    for (var user of users) {
      if (query != 0 && query.length <= user[0].length && query == user[0].substring(0, query.length).normalize('NFD').replace(/[\u0300-\u036f]/g, "").toLowerCase()) {
        $("#user-" + user[1]).show();
      } else {
        $("#user-" + user[1]).hide();
      }
    }
  });
  
  $(".user-match").click(event => {
    event.currentTarget.children[0].click();
  });
});