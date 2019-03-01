#include "soil_humidity_sensor.h"

SoilHumiditySensor::SoilHumiditySensor(uint8_t sensor_power_port, uint8_t sensor_data_port, uint8_t sensor_observer_port)
{
	power_port = sensor_power_port;
	data_port = sensor_data_port;
	observer_port = sensor_observer_port;
	last_humidity_reading = 0.0f;
	initialize_pwm(power_port);
	my_make_input(data_port);
	if (observer_port != 0xFF)
	{
		Serial.println("Using Vin observer for Soil Humidity Sensor");
		my_make_input(observer_port);
	}
}

SoilHumiditySensor::~SoilHumiditySensor()
{
}

void SoilHumiditySensor::update()
{
	float sum = 0.0f;
	float sum_vin = 0.0f;
	my_pwm(power_port, 255);
	delay(100);
	for (uint8_t i = 0; i < SOIL_HUMIDITY_SENSOR_AVERAGE_SIZE; i++)
	{
		sum += my_adc_volts(data_port);
		sum_vin += (observer_port != 0xFF) ? my_adc_volts(observer_port) : MY_ADC_MAX_VOLTAGE;
	}
	my_pwm(power_port, 0);
	last_humidity_reading = min(max(100.0f * (sum_vin - sum) / sum_vin, 0.0f), 100.0f);
}

float SoilHumiditySensor::get_humidity()
{
	return last_humidity_reading;
}
