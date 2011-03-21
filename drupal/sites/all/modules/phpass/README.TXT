The default password hashes are insecure.  MD5 has long been known to be
crackable, and we should not be storing passwords using this.

There has been a long discussion about this.  See http://drupal.org/node/29706.

So, this is a proof-of-concept module that implements secure password hashes
using the phpass password hashing method.  Hopefully, we can get something
like this into Drupal 7.

To download and install phpass, please see INSTALL.TXT.

