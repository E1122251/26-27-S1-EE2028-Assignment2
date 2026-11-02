#include "sensors.h"

#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_tsensor.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_psensor.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_hsensor.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_accelero.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_gyro.h"
#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_magneto.h"

extern UART_HandleTypeDef huart1;

static const float TEMP_UPPER_BOUND = 35.0;
static const float PRESSURE_UPPER_BOUND = 1500.0;
static const float HUMIDITY_UPPER_BOUND = 75.0;
static const float ACCEL_LOWER_BOUND = -9.0;
static const float GYRO_UPPER_BOUND = 30.0;
static const float MAGNETO_UPPER_BOUND = 1.0;

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

void updateSensorData(SensorData *sensors, uint32_t runtime) {

	if (runtime % 1000 != 0) return;

	// Temp
	sensors->temp.reading = readTemp();
	sensors->temp.warning = (sensors->temp.reading > TEMP_UPPER_BOUND);

	// Pressure
	sensors->pressure.reading = readPressure();
	sensors->pressure.warning = (sensors->pressure.reading > PRESSURE_UPPER_BOUND);

	// Humidity
	sensors->humidity.reading = readHumidity();
	sensors->humidity.warning = (sensors->humidity.reading > HUMIDITY_UPPER_BOUND);

	// Accel
	sensors->accel.reading = readAccel();
	sensors->accel.warning = (sensors->accel.reading < ACCEL_LOWER_BOUND);


	// Gyro
	sensors->gyro.reading = readGyro();
	sensors->gyro.warning = (sensors->gyro.reading > GYRO_UPPER_BOUND);

	// Magneto
	sensors->magneto.reading = readMagneto();
	sensors->magneto.warning = (sensors->magneto.reading > MAGNETO_UPPER_BOUND);

}

void transmitTelemetry(SensorData sensors, uint32_t runtime) {

	if (runtime % 1000 != 0) return;

	char message_transmit[128];

	if (sensors.temp.warning) {
		sprintf(message_transmit, "T: %.2f degC, exceeds threshold of %.2f degC\r\n", sensors.temp.reading, TEMP_UPPER_BOUND);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
	}

	if (sensors.pressure.warning) {
		sprintf(message_transmit, "P: %.2f hPa, exceeds threshold of %.2f hPa\r\n", sensors.pressure.reading, PRESSURE_UPPER_BOUND);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit), 0xFFFF);
	}

	if (sensors.humidity.warning) {
		sprintf(message_transmit, "H: %.2f %%rH, exceeds threshold of %.2f %%rH\r\n", sensors.humidity.reading, HUMIDITY_UPPER_BOUND);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit), 0xFFFF);
	}

	if (sensors.accel.warning) {
		sprintf(message_transmit, "A: %.2f m/s^2, exceeds threshold of %.2f m/s^2\r\n", sensors.accel.reading, ACCEL_LOWER_BOUND);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
	}

	if (sensors.gyro.warning) {
		sprintf(message_transmit, "G: %.2f deg/s, exceeds threshold of %.2f deg/s\r\n", sensors.gyro.reading, GYRO_UPPER_BOUND);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
	}

	if (sensors.magneto.warning) {
		sprintf(message_transmit, "M: %.2f G, exceeds threshold of %.2f G\r\n", sensors.magneto.reading, MAGNETO_UPPER_BOUND);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
	}

	bool noWarnings = !(
		sensors.temp.warning ||
		sensors.pressure.warning ||
		sensors.humidity.warning ||
		sensors.accel.warning ||
		sensors.gyro.warning ||
		sensors.magneto.warning
	);

	if (noWarnings) {

		sprintf(message_transmit, "T: %.2f degC, ", sensors.temp.reading);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);

		sprintf(message_transmit, "P: %.2f hPa, ", sensors.pressure.reading);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit), 0xFFFF);

		sprintf(message_transmit, "H: %.2f %%rH, ", sensors.humidity.reading);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit), 0xFFFF);

		sprintf(message_transmit, "A: %.2f m/s^2, ", sensors.accel.reading);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);

		sprintf(message_transmit, "G: %.2f deg/s, ", sensors.gyro.reading);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);

		sprintf(message_transmit, "M: %.2f G\r\n", sensors.magneto.reading);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);

	}
}

