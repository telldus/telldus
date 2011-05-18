#include <telldus-core.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

void WINAPI sensorEvent(const char *protocol, const char *model, int sensorId, int dataType, const char *value, int ts, int callbackId, void *context) {
	char timeBuf[80];
	time_t timestamp = ts;
	
	//Print the sensor
	printf("%s,\t%s,\t%i\n", protocol, model, sensorId);

	//Retrieve the values the sensor supports
	if (dataType == TELLSTICK_TEMPERATURE) {
		strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
		printf("Temperature:\t%sº\t(%s)\n", value, timeBuf);
		
	} else if (dataType == TELLSTICK_HUMIDITY) {
		strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
		printf("Humidity:\t%s%%\t(%s)\n", value, timeBuf);
	}
	printf("\n");
}

int main(void) {
	int callbackId = 0;
	
	tdInit();

	//Register for callback
	callbackId = tdRegisterSensorEvent( (TDSensorEvent)&sensorEvent, 0 );

	//Our own simple eventloop
	while(1) {
		sleep(100);
	}

	//Cleanup
	tdUnregisterCallback( callbackId );
	tdClose();

	return 0;
}
