# ACSF API Tools
A simple implementation of the Acquia Cloud Site Factory API in Drush.

### Setup
Add the following to your drush alias file (see https://github.com/drush-ops/drush/blob/master/examples/example.aliases.drushrc.php).

For example in ~/.drush/mysite.aliases.drushrc.php add:

```<?php
$aliases['api'] = array(
  'sfapi-user' => 'mysite.user',
  'sfapi-password' => 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX',
  'sfapi-site' => 'www.mysite.acsitefactory.com',
);
```
and enable the module

### Usage

####sf-connect (sfc) 
Returns the formatted URL used to connect.

example:

    > drush @mysite.api stc
    https://mysite.user:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX@www.mysite.acsitefactory.com/api/v1                                             [ok]
