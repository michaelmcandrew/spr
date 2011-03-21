phpbb_api.php integration module
==============================================================================

phpbb_api.php provides advanced integration with phpBB3 Forum
http://www.phpbb.com/.

Installation
------------------------------------------------------------------------------

1) Download the phpbb_api archive from http://vgb.org.ru
   Unpack phpbb_api archive.

2) Move phpbb_api.php, phpbb_api_subs.php, phpbb_api_recent.php
   to your phpbb root.

3) Include phpbb_api.php in your php code.

   require_once(dirname(__FILE__) .'/phpbb_api.php');

   Advanced

1) Move phpbb_api.php, phpbb_api_subs.php, phpbb_api_recent.php 
   to directory of your choice.
2) Define global $phpbb_boarddir; and set it to phpbb root.
4) Include phpbb_api.php in your php code.

    global $phpbb_root_path;
    $phpbb_root_path = 'path to your phpbb root';
    require_once ('path to your directory' .'/phpbb_api.php');

You may see advanced usage of phpbb_api.php function calls in Drupal phpbbforum.module code.

To see how phpbb_api.php works in phpBB3 + Drupal bridge you may visit
http://vgb.org.ru

Install first locally and ensure that it works as you expect.

Use at you risk on production site.

------------------------------------------------------------------------------
phpbb_api.php (Ñ) 2007-2008 by Vadim G.B. (http://vgb.org.ru)
------------------------------------------------------------------------------


