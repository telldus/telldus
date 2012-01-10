<?php

require_once 'common.php';

$consumer = new HTTP_OAuth_Consumer(constant('PUBLIC_KEY'), constant('PRIVATE_KEY'), $_SESSION['token'], $_SESSION['tokenSecret']);

try {
	$consumer->getAccessToken(constant('ACCESS_TOKEN'));
	
	$_SESSION['accessToken'] = $consumer->getToken();
	$_SESSION['accessTokenSecret'] = $consumer->getTokenSecret();

	header('Location:index.php');
} catch (Exception $e) {
	?>
		<p>Authorization failed!</p>
		<p><a href="index.php">Go back</a></p>
	<?php
}


