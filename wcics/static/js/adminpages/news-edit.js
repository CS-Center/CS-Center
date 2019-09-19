$(document).ready(() => {
  $("#delete-news-button").click(() => {
    if (confirm("Are you sure you want to delete this news item?")) {
      $("#delete").prop("checked", true);
      $("form")[0].submit.click();
    }
  });
});