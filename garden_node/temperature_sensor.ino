#include "relay.h"

TemperatureSensor::TemperatureSensor(uint8_t sensor_port)
{
	sensor = new AM2302(sensor_port);
	sensor->begin();
	last_temperature_reading = 0.0f;
}

TemperatureSensor::~TemperatureSensor()
{
}

void TemperatureSensor::update()
{
	sensor->readTemperature();
	last_temperature_reading = sensor->temperature_C;
}

float TemperatureSensor::get_temperature()
{
	return last_temperature_reading;
}

