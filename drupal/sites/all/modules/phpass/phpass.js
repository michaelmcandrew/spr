// $Id: phpass.js,v 1.1.4.2 2007/12/24 01:28:41 douggreen Exp $

if (Drupal.jsEnabled) {
  $(document).ready(
    function() {
      $('div.user-admin-hash-radios input[@type=radio]').click(function () {
        value = this.value == 'phpass' ? 1 : 0;
        $('div.user-admin-hash-settings')[['hide', 'show'][value]]();
      });
    }
  );
}
