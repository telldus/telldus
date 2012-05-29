<?php

require_once 'xpllighting.class.php';

$xplinstance = new XPLLighting('192.168.0.50');

$running = true;
declare(ticks = 1) {
	pcntl_signal( SIGINT, 'signalHandler' );
	while($running) {
		if ($xplinstance->doEvents()) {
			usleep(100);
		}
	}
	$xplinstance->detach();
}

function signalHandler() {
global $running;
	$running = false;
}
