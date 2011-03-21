/* $Id: weblinks_checker.js,v 1.1.2.3 2009/07/30 15:16:55 nancyw Exp $ */

function weblinks_checker_handler(event) {
  if ($("input[@name=weblinks_checker_enabled]:checked").val() == 1) {
    $("div.weblinks_checker_hide").show();
    $("#edit-weblinks-checker-enabled-wrapper label").css({fontWeight:"normal" });
  }
  else {
    $("div.weblinks_checker_hide").hide();
    $("#edit-weblinks-checker-enabled-wrapper label").css({fontWeight:"bold" });
  }

  if ($("input[@name=weblinks_checker_unpublish]:checked").val() == 0) {
    $("div.weblinks_checker_unpublish_hide").hide();
  }
  else {
    $("div.weblinks_checker_unpublish_hide").show();
  }
}

// Run the javascript on page load.
if (Drupal.jsEnabled) {
  $(document).ready(function () {
    // On page load, determine the default settings.
    weblinks_checker_handler();

    // Bind the functions to click events.
    $("input[@name=weblinks_checker_enabled]").bind("click", weblinks_checker_handler);
    $("input[@name=weblinks_checker_unpublish]").bind("click", weblinks_checker_handler);
  });
}