#pragma once

#include "arduino_wrapper.h"

#define SOIL_HUMIDITY_SENSOR_AVERAGE_SIZE 128

class SoilHumiditySensor
{
	public:
		SoilHumiditySensor(uint8_t power_port, uint8_t data_port, uint8_t sensor_observer_port = 0xFF);
		~SoilHumiditySensor();
		float get_humidity();
		void update();
	private:
		uint8_t power_port;
		uint8_t data_port;
		uint8_t observer_port;
		float last_humidity_reading;
};
