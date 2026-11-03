#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

#include "sensorData.h"

SensorData getSensorData(void);
void updateSensorData(void);
void transmitTelemetry(void);

#endif
