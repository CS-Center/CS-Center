$(document).ready(() => {
  $("#sandbox-field").keyup(event => {
    window.history.replaceState({}, "", "?query=" + window.btoa($("#sandbox-field").val()).replace(/\+/g, "%2B"));
    if (event.ctrlKey && event.keyCode == 13) {
      $("#preview-update-button").click();
    }
  });
})