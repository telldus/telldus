<?php

session_start();

require_once 'HTTP/OAuth/Consumer.php';

define('PUBLIC_KEY', '');
define('PRIVATE_KEY', '');

define('URL', 'http://api.telldus.net');
define('REQUEST_TOKEN', constant('URL').'/oauth/requestToken');
define('AUTHORIZE_TOKEN', constant('URL').'/oauth/authorize');
define('ACCESS_TOKEN', constant('URL').'/oauth/accessToken');
define('REQUEST_URI', constant('URL').'/xml');

define('BASE_URL', 'http://'.$_SERVER["SERVER_NAME"].dirname($_SERVER['REQUEST_URI']));

define('TELLSTICK_TURNON', 1);
define('TELLSTICK_TURNOFF', 2);