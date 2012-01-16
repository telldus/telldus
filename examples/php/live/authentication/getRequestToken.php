<?php

require_once 'common.php';

$consumer = new HTTP_OAuth_Consumer(constant('PUBLIC_KEY'), constant('PRIVATE_KEY'));

$consumer->getRequestToken(constant('REQUEST_TOKEN'), constant('BASE_URL').'/getAccessToken.php');

$_SESSION['token'] = $consumer->getToken();
$_SESSION['tokenSecret'] = $consumer->getTokenSecret();

$url = $consumer->getAuthorizeUrl(constant('AUTHORIZE_TOKEN'));
header('Location:'.$url);
