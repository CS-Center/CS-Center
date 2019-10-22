$(document).ready(() => {
  $("#delete-lesson-button").click(() => {
    if (confirm("Are you sure you want to delete this lesson?")) {
      $("#delete").prop("checked", true);
      $("form")[0].submit.click();
    }
  });
});