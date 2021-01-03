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
});