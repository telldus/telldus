#include <telldus-core.h>
#include <time.h>
#include <stdio.h>

const int DATA_LENGTH = 20;

int main(void) {
	char protocol[DATA_LENGTH], model[DATA_LENGTH];
	int sensorId = 0, dataTypes = 0;
	char value[DATA_LENGTH];
	char timeBuf[80];
	time_t timestamp = 0;

	
	tdInit();

	while(tdSensor(protocol, DATA_LENGTH, model, DATA_LENGTH, &sensorId, &dataTypes) == TELLSTICK_SUCCESS) {
		//Print the sensor
		printf("%s,\t%s,\t%i\n", protocol, model, sensorId);

		//Retrieve the values the sensor supports
		if (dataTypes & TELLSTICK_TEMPERATURE) {
			tdSensorValue(protocol, model, sensorId, TELLSTICK_TEMPERATURE, value, DATA_LENGTH, (int *)&timestamp);
			strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
			printf("Temperature:\t%sº\t(%s)\n", value, timeBuf);
		}
		if (dataTypes & TELLSTICK_HUMIDITY) {
			tdSensorValue(protocol, model, sensorId, TELLSTICK_HUMIDITY, value, DATA_LENGTH, (int *)&timestamp);
			strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
			printf("Humidity:\t%s%%\t(%s)\n", value, timeBuf);
		}
		printf("\n");
	}

	tdClose();

	return 0;
}
