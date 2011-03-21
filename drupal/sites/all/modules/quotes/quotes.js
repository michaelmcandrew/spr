/* $Id: quotes.js,v 1.1.4.2 2008/08/23 17:15:55 nancyw Exp $ */

function quotes_more_handler(event) {
  if ($("input[@name=block_type]:checked").val() == '0') {
    $("input[@name=block_more]").parents("div.quotes_block_more").show();
  }
  else {
    $("input[@name=block_more]").parents("div.quotes_block_more").hide();
  }
}

// Run the javascript on page load.
if (Drupal.jsEnabled) {
  $(document).ready(function () {
  // On page load, determine the default settings.
  quotes_more_handler();

  // Bind the functions to click events.
  $("input[@name=block_type]").bind("click", quotes_more_handler);
  });
}