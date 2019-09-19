$(document).ready(() => {
  $(".user-select-button").click(event => {
    add_author(parseInt(event.currentTarget.getAttribute("data-uid")));
  });
  
  update_authors_field();
  authors.forEach(add_author);
});

function add_author(uid) {
  if ($("#author-" + uid).length == 0) $("#author-list").append(
                                       $(".author-" + uid)
                                         .clone()
                                         .attr("id", "author-" + uid)
                                         .click(event => remove_author(parseInt(event.currentTarget.getAttribute("data-uid")))
                                       )
                                     );
  if (!authors.includes(uid)) { authors.push(uid); update_authors_field(); }
}

function remove_author(uid) {
  $("#author-" + uid).remove();
  authors.splice(authors.indexOf(uid), 1);
  update_authors_field();
}

function update_authors_field() {
  $("#authors").val(authors.join(" "));
}