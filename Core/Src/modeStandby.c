#include "modeStandby.h"

#include <stdio.h>

#include "sensorData.h"
#include "transmit.h"

static const float TEMP_UPPER_BOUND = 35.0;
static const float PRESSURE_UPPER_BOUND = 1500.0;
static const float HUMIDITY_UPPER_BOUND = 75.0;
static const float ACCEL_LOWER_BOUND = -9.0;
static const float GYRO_UPPER_BOUND = 30.0;
static const float MAGNETO_UPPER_BOUND = 1.0;

void MS_transmitTelemetry(SensorData sensorData) {
	char message_transmit[128];

	if (sensorData.temp.warning) {
		sprintf(message_transmit, "T: %.2f degC, above threshold of %.2f degC\r\n", sensorData.temp.reading, TEMP_UPPER_BOUND);
		transmit(message_transmit);
	}

	if (sensorData.pressure.warning) {
		sprintf(message_transmit, "P: %.2f hPa, above threshold of %.2f hPa\r\n", sensorData.pressure.reading, PRESSURE_UPPER_BOUND);
		transmit(message_transmit);
	}

	if (sensorData.humidity.warning) {
		sprintf(message_transmit, "H: %.2f %%rH, above threshold of %.2f %%rH\r\n", sensorData.humidity.reading, HUMIDITY_UPPER_BOUND);
		transmit(message_transmit);
	}

	if (sensorData.accel.warning) {
		sprintf(message_transmit, "A: %.2f m/s^2, below threshold of %.2f m/s^2\r\n", sensorData.accel.reading, ACCEL_LOWER_BOUND);
		transmit(message_transmit);
	}

	if (sensorData.gyro.warning) {
		sprintf(message_transmit, "G: %.2f deg/s, above threshold of %.2f deg/s\r\n", sensorData.gyro.reading, GYRO_UPPER_BOUND);
		transmit(message_transmit);
	}

	if (sensorData.magneto.warning) {
		sprintf(message_transmit, "M: %.2f G, above threshold of %.2f G\r\n", sensorData.magneto.reading, MAGNETO_UPPER_BOUND);
		transmit(message_transmit);
	}

	if (sensorData.noWarnings) {

		sprintf(message_transmit, "T: %.2f degC, ", sensorData.temp.reading);
		transmit(message_transmit);

		sprintf(message_transmit, "P: %.2f hPa, ", sensorData.pressure.reading);
		transmit(message_transmit);

		sprintf(message_transmit, "H: %.2f %%rH, ", sensorData.humidity.reading);
		transmit(message_transmit);

		sprintf(message_transmit, "A: %.2f m/s^2, ", sensorData.accel.reading);
		transmit(message_transmit);

		sprintf(message_transmit, "G: %.2f deg/s, ", sensorData.gyro.reading);
		transmit(message_transmit);

		sprintf(message_transmit, "M: %.2f G\r\n", sensorData.magneto.reading);
		transmit(message_transmit);

	}
}

void MS_transmitLifetime(uint32_t runtime) {
	int lifetime = runtime / 1000;
	char message_transmit[128];

	sprintf(message_transmit, "Drone has been active for %i seconds\r\n", lifetime);
	transmit(message_transmit);
}
