#include "luminosity_sensor.h"

LuminositySensor::LuminositySensor(uint8_t sensor_power_port, uint8_t sensor_data_port, uint8_t sensor_observer_port)
{
	power_port = sensor_power_port;
	data_port = sensor_data_port;
	observer_port = sensor_observer_port;
	last_luminosity_reading = 0.0f;
	initialize_pwm(power_port);
	my_make_input(data_port);
	if (observer_port != 0xFF)
	{
		Serial.println("Using Vin observer for Luminosity Sensor");
		my_make_input(observer_port);
	}
}

LuminositySensor::~LuminositySensor()
{
}

void LuminositySensor::update()
{
	float sum = 0.0f;
	float sum_vin = 0.0f;
	my_pwm(power_port, 255);
	delay(100);
	for (uint32_t i = 0; i < LUMINOSITY_SENSOR_AVERAGE_SIZE; i++)
	{
		sum += my_adc_volts(data_port);
		sum_vin += (observer_port != 0xFF) ? my_adc_volts(observer_port) : MY_ADC_MAX_VOLTAGE;
	}
	my_pwm(power_port, 0);
	sum_vin /= LUMINOSITY_SENSOR_AVERAGE_SIZE;
	sum /= LUMINOSITY_SENSOR_AVERAGE_SIZE;
	last_luminosity_reading = max(LUMINOSITY_SENSOR_VOLTAGE_DIVIDER_R1 / sum * (sum_vin - sum), 0.0f);
}

float LuminositySensor::get_luminosity()
{
	return last_luminosity_reading;
}
