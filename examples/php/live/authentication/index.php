<?php

require_once 'common.php';

if (isset($_GET['clear'])) {
	session_destroy();
	header('location:index.php');
	exit();
}

if (!isset($_SESSION['accessToken'])) {
	?>We have no access token, <a href="getRequestToken.php">connect us</a><?php
	exit();
}

?>
	<p>We have access!</p>
	<p>
		In your system, store these values to do requests for this user:<br>
		Token: <?php echo $_SESSION['accessToken']; ?><br>
		Secret: <?php echo $_SESSION['accessTokenSecret']; ?>
	</p>
	<p><a href="index.php?clear">Clear the token and restart</a></p>
	<p><a href="index.php?listDevices">List users devices</a></p>
<?php

if (isset($_GET['listDevices'])) {
	$consumer = new HTTP_OAuth_Consumer(constant('PUBLIC_KEY'), constant('PRIVATE_KEY'), $_SESSION['accessToken'], $_SESSION['accessTokenSecret']);
	$params = array(
		'supportedMethods' => constant('TELLSTICK_TURNON') | constant('TELLSTICK_TURNOFF'),
	);
	$response = $consumer->sendRequest(constant('REQUEST_URI').'/devices/list', $params, 'GET');
	echo '<pre>';
	echo( htmlentities($response->getBody()));
}

?><p><a href="index.php?listClients">List users clients</a></p><?php

if (isset($_GET['listClients'])) {
	$consumer = new HTTP_OAuth_Consumer(constant('PUBLIC_KEY'), constant('PRIVATE_KEY'), $_SESSION['accessToken'], $_SESSION['accessTokenSecret']);
	$params = array();
	$response = $consumer->sendRequest(constant('REQUEST_URI').'/clients/list', $params, 'GET');
	echo '<pre>';
	echo( htmlentities($response->getBody()));
}


