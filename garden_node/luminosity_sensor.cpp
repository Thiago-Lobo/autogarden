#include "luminosity_sensor.h"

LuminositySensor::LuminositySensor(uint8_t sensor_power_port, uint8_t sensor_data_port)
{
	power_port = sensor_power_port;
	data_port = sensor_data_port;
	last_luminosity_reading = 0.0f;
	ag_make_output(power_port);
	ag_make_input(data_port);
}

LuminositySensor::~LuminositySensor()
{
}

void LuminositySensor::update()
{
	float sum = 0.0f;
	ag_pwm(power_port, 255);
	for (uint8_t i = 0; i < LUMINOSITY_SENSOR_AVERAGE_SIZE; i++)
	{
		sum += ag_adc_volts(data_port);
	}
	ag_pwm(power_port, 0);
	sum = ADC_MAX_VOLTAGE - (sum / LUMINOSITY_SENSOR_AVERAGE_SIZE);
	last_luminosity_reading = (sum * LUMINOSITY_SENSOR_VOLTAGE_DIVIDER_R1) / (ADC_MAX_VOLTAGE - sum);
}

float LuminositySensor::get_luminosity()
{
	return last_luminosity_reading;
}

