var create = false;

$(document).ready(() => {
  $(".listen").on("input", event => $("#save-all").prop("disabled", false));
  
  $(".month").on("input", event => {
    var target = event.currentTarget;
    var day = $("#" + target.id.replace(/month/, "day"));
    var year = $("#" + target.id.replace(/month/, "year"));
    
    var maxday = [31, year.val() % 4 == 0 ? (year.val() % 100 == 0 ? (year.val() % 400 == 0 ? 29 : 28) : 29) : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31][target.value];
    
    day.prop("max", maxday);
    
    if (day.val() > maxday) {
      day.val(maxday);
    }
  });
  
  $(".year").on("input", event => {
    var target = event.currentTarget;
    var day = $("#" + target.id.replace(/year/, "day"));
    var month = $("#" + target.id.replace(/year/, "month"));
    
    if (month.val() == 1) {
      var maxday = target.value % 4 == 0 ? (target.value % 100 == 0 ? (target.value % 400 == 0 ? 29 : 28) : 29) : 28;
      
      day.prop("max", maxday);
      
      if (day.val() > maxday) {
        day.val(maxday);
      }
    }
  });
  
  $("#save-all-create").click(() => {
    create = true;
    $("#attendance-form").submit();
  });
  
  $("#attendance-form").submit(() => {
    var changes = [];
    
    $(".code-edit").each((index, element) => {
      var i = index + 1;
      var id = element.getAttribute("data-id");
      var code = $("#code-" + i).val();
      var del = $("#delete-" + i).is(":checked");
      
      if (del) {
        changes.push({
          "id": parseInt(id),
          "delete": true
        });
      } else {
        var start = Math.floor(new Date(
          parseInt($("#start-year-" + i).val()),
          parseInt($("#start-month-" + i).val()),
          parseInt($("#start-day-" + i).val()),
          parseInt($("#start-hour-" + i).val()),
          parseInt($("#start-minute-" + i).val()),
          parseInt($("#start-second-" + i).val())
        ) / 1000);
        var end = Math.floor(new Date(
          parseInt($("#end-year-" + i).val()),
          parseInt($("#end-month-" + i).val()),
          parseInt($("#end-day-" + i).val()),
          parseInt($("#end-hour-" + i).val()),
          parseInt($("#end-minute-" + i).val()),
          parseInt($("#end-second-" + i).val())
        ) / 1000);

        var code_changed = code != element.getAttribute("data-code");
        var start_changed = start != parseInt(element.getAttribute("data-start"));
        var end_changed = end != parseInt(element.getAttribute("data-end"));

        if (code_changed || start_changed || end_changed) {
          var change = {"id": parseInt(id)};

          if (code_changed) change.code = code;
          if (start_changed) change.start = start;
          if (end_changed) change.end = end;

          changes.push(change);
        }
      }
    });
    
    $("#attendance-form").append($("<input />", {
      type: "hidden",
      name: "changes",
      value: JSON.stringify(changes)
    }));
    
    $("#attendance-form").append($("<input />", {
      type: "hidden",
      name: "create",
      value: create
    }));
    
    return true;
  });
});