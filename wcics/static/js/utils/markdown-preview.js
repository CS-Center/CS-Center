$(document).ready(() => {
  $("#preview-update-button").click(event => {
    POST("/markdown/", {
      html: $("#" + event.currentTarget.getAttribute("data-source")).val(),
      mathjax: true
    }).then(a => {
      var element = $("#markdown-preview");
      element.html(a);
      MathJax.Hub.Typeset(element[0]);
      document.querySelectorAll("pre code[class|=language]").forEach(hljs.highlightBlock);
    });
  });
  
  $("#preview-update-button").click();
});