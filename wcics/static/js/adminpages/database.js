$(document).ready(() => {
  $("#query").keyup(event => {
    window.history.replaceState({}, "", "?query=" + window.btoa($("#query").val()).replace(/\+/g, "%2B"));
  });
});