<?php


if(!extension_loaded('telldus')) {
	dl('telldus.' . PHP_SHLIB_SUFFIX);
}

$devices = tdGetNumberOfDevices();
printf("Devices: %d\n", $devices);

$allMethods = TELLDUS_TURNON | TELLDUS_TURNOFF | TELLDUS_BELL | TELLDUS_DIM;

for( $i = 0; $i < $devices; ++$i ) {
	$id = tdGetDeviceId( $i );
	$name = utf8_encode(tdGetName( $id ));
	printf("%s - %s\n", $id, $name);

	$methods = tdMethods( $id, $allMethods );
	if ($methods & TELLDUS_TURNON) {
		echo " * TurnOn\n";
		tdTurnOn( $id );
		sleep(1);
	}
	if ($methods & TELLDUS_TURNOFF) {
		echo " * TurnOff\n";
		tdTurnOff( $id );
		sleep(1);
	}
	if ($methods & TELLDUS_BELL) {
		echo " * Bell\n";
		tdBell( $id );
		sleep(1);
	}
	if ($methods & TELLDUS_TOGGLE) {
		echo " * Toggle\n";
	}
	if ($methods & TELLDUS_DIM) {
		echo " * Dim\n";
		tdDim( $id, 128 );
		sleep(1);
	}
}

