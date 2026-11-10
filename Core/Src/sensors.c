#include "sensors.h"

#include <math.h>
#include <stdio.h>

#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_tsensor.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_psensor.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_hsensor.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_accelero.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_gyro.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_magneto.h"

static SensorData sensorData = { 0 };

static const float TEMP_UPPER_BOUND = 35.0;
static const float PRESSURE_UPPER_BOUND = 1500.0;
static const float HUMIDITY_UPPER_BOUND = 75.0;
static const float ACCEL_LOWER_BOUND = -9.0;
static const float GYRO_UPPER_BOUND = 30.0;
static const float MAGNETO_UPPER_BOUND = 1.0;

SensorData getSensorData(void) {
	return sensorData;
}

static float readTemp(void) {
	float temp_data;
	temp_data = BSP_TSENSOR_ReadTemp();

	return temp_data;
}

static float readPressure(void) {
	float pressure_data;
	pressure_data = BSP_PSENSOR_ReadPressure();

	return pressure_data;
}

static float readHumidity(void) {
	float humidity_data;
	humidity_data = BSP_HSENSOR_ReadHumidity();

	return humidity_data;
}

static float readAccel(void) {
	float accel_data[3];
	int16_t accel_data_i16[3] = { 0 };
	BSP_ACCELERO_AccGetXYZ(accel_data_i16);

	accel_data[0] = (float)accel_data_i16[0] * (9.8/1000.0f);
	accel_data[1] = (float)accel_data_i16[1] * (9.8/1000.0f);
	accel_data[2] = (float)accel_data_i16[2] * (9.8/1000.0f);

	return accel_data[2];
}

static float readGyro(void) {
	float gyro_data[3];
	BSP_GYRO_GetXYZ(gyro_data);

	gyro_data[0] /= 1000.0f;
	gyro_data[1] /= 1000.0f;
	gyro_data[2] /= 1000.0f;

	float gyro_magnitude = sqrtf(gyro_data[0] * gyro_data[0] + gyro_data[1] * gyro_data[1] + gyro_data[2] * gyro_data[2]);
	return gyro_magnitude;
}

static float readMagneto(void) {
	float magneto_data[3];
	int16_t magneto_data_i16[3] = { 0 };
	BSP_MAGNETO_GetXYZ(magneto_data_i16);

	magneto_data[0] = (float)magneto_data_i16[0] / (6842.0f);
	magneto_data[1] = (float)magneto_data_i16[1] / (6842.0f);
	magneto_data[2] = (float)magneto_data_i16[2] / (6842.0f);

	float magneto_magnitude = sqrtf(magneto_data[0] * magneto_data[0] + magneto_data[1] * magneto_data[1] + magneto_data[2] * magneto_data[2]);
	return magneto_magnitude;
}

void updateSensorData(void) {

	// Temp
	sensorData.temp.reading = readTemp();
	sensorData.temp.warning = (sensorData.temp.reading > TEMP_UPPER_BOUND);

	// Pressure
	sensorData.pressure.reading = readPressure();
	sensorData.pressure.warning = (sensorData.pressure.reading > PRESSURE_UPPER_BOUND);

	// Humidity
	sensorData.humidity.reading = readHumidity();
	sensorData.humidity.warning = (sensorData.humidity.reading > HUMIDITY_UPPER_BOUND);

	// Accel
	sensorData.accel.reading = readAccel();
	sensorData.accel.warning = (sensorData.accel.reading < ACCEL_LOWER_BOUND);

	// Gyro
	sensorData.gyro.reading = readGyro();
	sensorData.gyro.warning = (sensorData.gyro.reading > GYRO_UPPER_BOUND);

	// Magneto
	sensorData.magneto.reading = readMagneto();
	sensorData.magneto.warning = (sensorData.magneto.reading > MAGNETO_UPPER_BOUND);

	// noWarnings
	sensorData.noWarnings = !(
		sensorData.temp.warning ||
		sensorData.pressure.warning ||
		sensorData.humidity.warning ||
		sensorData.accel.warning ||
		sensorData.gyro.warning ||
		sensorData.magneto.warning
	);

}
