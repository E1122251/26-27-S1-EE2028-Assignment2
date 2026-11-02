#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

#include "sensorData.h"

void updateSensorData(SensorData *sensors, uint32_t runtime);
void transmitTelemetry(SensorData sensors, uint32_t runtime);

#endif
