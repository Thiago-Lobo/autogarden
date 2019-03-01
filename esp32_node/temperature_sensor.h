#pragma once

#include "arduino_wrapper.h"
#include "DHT.h"

#define TEMPERATURE_SENSOR_TYPE DHT22

class TemperatureSensor
{
	public:
		TemperatureSensor(uint8_t power_port, uint8_t sensor_port);
		~TemperatureSensor();
		float get_temperature();
		void update();
	private:
		uint8_t power_port;
		DHT* sensor;
		float last_temperature_reading;
};
