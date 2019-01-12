#include "relay.h"

SoilHumiditySensor::SoilHumiditySensor(uint8_t sensor_power_port, uint8_t sensor_data_port)
{
	power_port = sensor_power_port;
	data_port = sensor_data_port;
	last_humidity_reading = 0.0f;
	ag_make_output(power_port);
	ag_make_input(data_port);
}

SoilHumiditySensor::~SoilHumiditySensor()
{
}

void SoilHumiditySensor::update()
{
	float sum = 0.0f;
	ag_pwm(power_port, 255);
	for (uint8_t i = 0; i < SOIL_HUMIDITY_SENSOR_AVERAGE_SIZE; i++)
	{
		sum += ag_adc(data_port);
	}
	ag_pwm(power_port, 0);
	last_humidity_reading = 1023.0 - sum / SOIL_HUMIDITY_SENSOR_AVERAGE_SIZE;
}

float SoilHumiditySensor::get_humidity()
{
	return last_humidity_reading;
}

