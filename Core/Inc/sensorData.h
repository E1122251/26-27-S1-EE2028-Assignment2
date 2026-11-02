#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <stdbool.h>

typedef struct {
	float reading;
	bool warning;
} IndividualSensorParameters;

typedef struct {
	IndividualSensorParameters temp;
	IndividualSensorParameters pressure;
	IndividualSensorParameters humidity;
	IndividualSensorParameters accel;
	IndividualSensorParameters gyro;
	IndividualSensorParameters magneto;
} SensorData;

#endif
