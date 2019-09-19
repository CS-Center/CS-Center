var target = null;

function drop_children(id) {
  $("#item-" + id).parent().append($("#item-" + id + " > ul").children().detach());
}

$(document).ready(() => {
  $(".topic-delete").click(event => {
    var id = event.currentTarget.getAttribute("data-id");
    var name = event.currentTarget.getAttribute("data-name");
    var action = event.currentTarget.getAttribute("data-action-ext");
    if (confirm("Are you sure you want to delete '" + name + "'" + (action == "" ? " and all of its descendants?" : "?"))) {
      POST("/admin/topic-delete" + action + "/" + id, {
        "token": $("#csrf_token").val()
      }).then(() => {
        if (action != "") {
          drop_children(id);
        }
        $("#item-" + id).remove();
      });
    }
  });
  
  $(".topic-move").click(event => {
    if (target === null) {
      target = event.currentTarget;
      var target_id = target.getAttribute("data-id");
      $("#move-keep-" + target_id).html("CANCEL");
      $("#move-" + target_id).html("CANCEL");
      $(".topic-move.keep").prop("disabled", true);
      $(".topic-move:not(.keep):not(#move-" + target_id + ")").html("SET AS PARENT");
    } else {
      var id = event.currentTarget.getAttribute("data-id");
      var tid = target.getAttribute("data-id");
      var action = target.getAttribute("data-action-ext");
      
      $("#move-keep-" + tid).html("MOVE &amp; LEAVE CHILDREN");
      $("#move-" + tid).html("MOVE");
      
      $(".topic-move").prop("disabled", false);
      $(".topic-move:not(.keep)").html("MOVE")
      
      if (id != tid) {
        if ($("#item-" + tid + " #item-" + id).length) {
          alert("Cannot invert hierarchy in one step! Please explicity reorganize the hierarchy in multiple steps.");
        } else if ($("#list-" + id + " > #item-" + tid).length) {
          // No change
        } else {
          POST("/admin/topic-move" + action + "/" + tid + "/" + id, {
            "token": $("#csrf_token").val()
          }).then(() => {
            window.location.reload();
          })
        }
      }
      
      target = null;
    }
  });
})