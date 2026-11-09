#ifndef MODESTANDBY_H
#define MODESTANDBY_H

#include "sensorData.h"

#include <stdint.h>

void MS_transmitTelemetry(SensorData sensorData);
void MS_transmitLifetime(uint32_t runtime);

#endif
