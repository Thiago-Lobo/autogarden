#pragma once

#include "arduino_wrapper.h"
#include "cactus_io_AM2302.h"

class TemperatureSensor
{
	public:
		TemperatureSensor(uint8_t sensor_port);
		~TemperatureSensor();
		float get_temperature();
		void update();
	private:
		AM2302* sensor;
		float last_temperature_reading;
};
