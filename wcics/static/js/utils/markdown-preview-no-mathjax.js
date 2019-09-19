$(document).ready(() => {
  $("#preview-update-button").click(event => {
    POST("/markdown/", {
      html: $("#" + event.currentTarget.getAttribute("data-source")).val(),
      mathjax: false
    }).then(a => {
      $("#markdown-preview").html(a);
      document.querySelectorAll("pre code[class|=language]").forEach(hljs.highlightBlock);
    });
  });
  
  $("#preview-update-button").click();
});