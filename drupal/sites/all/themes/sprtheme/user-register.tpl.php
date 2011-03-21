<div id="registration_form">
  <div class="field">
    <?php
      print drupal_render($form['account']['name']); // prints the username field
    ?>
  </div>
  <div class="field">
    <?php
      print drupal_render($form['account']['pass']); // print the password field
    ?>
  </div>
  <div class="field">
    <?php
        print drupal_render($form['submit']); // print the submit button
      ?>
    </div>
  </div>
</div>