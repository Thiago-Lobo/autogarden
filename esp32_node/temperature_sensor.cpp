#include "temperature_sensor.h"

TemperatureSensor::TemperatureSensor(uint8_t sensor_power_port, uint8_t sensor_port)
{
	power_port = sensor_power_port;
	
	initialize_pwm(power_port);
	my_pwm(power_port, 255);
	my_delay(100);
	
	sensor = new DHT(sensor_port, TEMPERATURE_SENSOR_TYPE);
	sensor->begin();
	
	last_temperature_reading = 0.0f;
}

TemperatureSensor::~TemperatureSensor()
{
}

void TemperatureSensor::update()
{
	last_temperature_reading = sensor->readTemperature();
}

float TemperatureSensor::get_temperature()
{
	return last_temperature_reading;
}
