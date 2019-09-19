$(document).ready(() => {
  update_all();
  window.setInterval(update_all, 1000);
  
  $(".time-control-field").on("input", event => {
    var name_query = "#" + event.currentTarget.getAttribute("data-name");
    update_desc(name_query, name_query + "_desc");
  });
  
  $(".time-control").click(function(event) {
    var name_query = "#" + event.currentTarget.getAttribute("data-name");
    var time = event.currentTarget.getAttribute("data-time");
    switch (time) {
      case "clear":
        $(name_query).val(0);
        break;
      case "forever":
        $(name_query).val(-1);
        break;
      case "current":
        fill_now(name_query);
        break;
      case "inc-hour":
        alter_time(name_query, 60 * 60);
        break;
      case "dec-hour":
        alter_time(name_query, -60 * 60);
        break;
      case "inc-day":
        alter_time(name_query, 24 * 60 * 60);
        break;
      case "dec-day":
        alter_time(name_query, -24 * 60 * 60);
        break;
      case "inc-week":
        alter_time(name_query, 7 * 24 * 60 * 60);
        break;
      case "dec-week":
        alter_time(name_query, -7 * 24 * 60 * 60);
        break;
    }
    update_desc(name_query, name_query + "_desc");
  });
});

function update_all() {
    update_desc("#access_can_login_after", "#access_can_login_after_desc");
}

function time_delta_string(time) {
  time = Math.ceil(time / 1000);
  var seconds = time % 60;
  time = Math.floor(time / 60);
  var minutes = time % 60;
  time = Math.floor(time / 60);
  var hours = time % 24;
  time = Math.floor(time / 24);
  var days = time % 7;
  time = Math.floor(time / 7);
  var weeks = time;
  
  var str = "";
  var carry = false;
  
  if (weeks && (carry = true)) str += weeks + "w";
  if (carry || days && (carry = true)) str += days + "d ";
  if (carry || hours && (carry = true)) str += hours + ":";
  if (carry || minutes && (carry = true)) str += (minutes < 10 ? "0" : "") + minutes + ":";
  if (carry || seconds) str += (seconds < 10 ? "0" : "") + seconds;
  
  return str;
}

function update_desc(src, dest) {
  var val = parseInt($(src).val());
  if (!isNaN(val)) {
    var date = new Date(val * 1000);
    var now = new Date();
    $(dest).html("(" + (val == 0 ? "N/A" : val == -1 ? "Forever" : date.toLocaleString() + (date >= now ? " - " + time_delta_string(date - now) + " remaining" : "")) + ")");
  }
}

function fill_now(query) {
  $(query).val(Math.floor(new Date().getTime() / 1000));
}

function alter_time(query, amount) {
  $(query).val(parseInt($(query).val()) + amount);
}