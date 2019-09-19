function receive(url) {
  return (text, status, data) => {
    $("#request-output").html(
      "(" + (data.redirected ? (url.startsWith("/") ? new URL(window.location.href).origin + url : url) + " &rarr; " : "") + data.url + ")<br /><br />" +
      "STATUS: " + status + "<br /><br />" +
      (data.redirected ? "REDIRECTED<br /><br />" : "") +
      "HEADERS" + "<ul>" + [...data.headers].map(a => "<li>" + a[0] + ": " + a[1] + "</li>").join("") + "</ul>" + "<br /><br /><br /><br />" +
      "<div class='small-text force-wrap'>" + escapeHtml(text).replace(/\n/g, "<br/>").replace(/  +/g, a => "&nbsp;".repeat(a.length)) + "</div>"
    );
  }
}

$(document).ready(() => {
  $("#request-type").on("change", () => {
    if ($("#request-type").val() == "post") {
      $("#post-data").show();
      $("label[for='post-data']").show();
    } else {
      $("#post-data").hide();
      $("label[for='post-data']").hide();
    }
  });

  $(".requests-sandbox-field").keyup(event => {
    if (event.ctrlKey && event.keyCode == 13) {
      $("#submit").click();
    }
  });
  
  $("#submit").click(event => {
    var url = $("#url").val().trim();
    var true_url = url.startsWith("/") ? new URL(window.location.href).origin + url : url;
    var recv = receive(url);
    var promise;
    
    if ($("#request-type").val() == "get") {
      promise = GET(url);
    } else {
      promise = POST(url, JSON.parse($("#post-data").val()));
    }
    
    promise.then(recv).catch(recv);
  })
});