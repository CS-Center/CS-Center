function show_helpbox(title, entries) {
  $("#helpboxtitle").html(title);
  $("#helpbox-entries").empty();
  entries.forEach(entry => {
    $("#helpbox-entries").append(
      $("<span>").append(
        $("<kbd>").html(entry[0])
      ).append(
        $("<span>").addClass("helpboxtext").html(entry[1])
      ).append(
        $("<br>")
      )
    );
  });
  $("#helpbox").show();
}

function hide_helpbox() {
  $("#helpbox").hide();
}