<?php


if(!extension_loaded('telldus')) {
	dl('telldus.' . PHP_SHLIB_SUFFIX);
}

$devices = telldus_get_number_of_devices();

for( $i = 0; $i < $devices; ++$i ) {
	$id = telldus_get_device_id( $i );
	$name = utf8_encode(telldus_get_name( $id ));
	printf("%s - %s\n", $id, $name);

	$methods = telldus_dev_methods( $id );
	if ($methods & TELLDUS_TURNON) {
		echo " * TurnOn\n";
		telldus_dev_turn_on( $id );
		sleep(1);
	}
	if ($methods & TELLDUS_TURNOFF) {
		echo " * TurnOff\n";
		telldus_dev_turn_off( $id );
		sleep(1);
	}
	if ($methods & TELLDUS_BELL) {
		echo " * Bell\n";
		telldus_dev_bell( $id );
		sleep(1);
	}
	if ($methods & TELLDUS_TOGGLE) {
		echo " * Toggle\n";
	}
	if ($methods & TELLDUS_DIM) {
		echo " * Dim\n";
		telldus_dev_dim( $id, 128 );
		sleep(1);
	}
}
