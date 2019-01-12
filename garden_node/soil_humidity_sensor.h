#pragma once

#include "arduino_wrapper.h"

#define SOIL_HUMIDITY_SENSOR_AVERAGE_SIZE 50

class SoilHumiditySensor
{
	public:
		SoilHumiditySensor(uint8_t power_port, uint8_t data_port);
		~SoilHumiditySensor();
		float get_humidity();
		void update();
	private:
		uint8_t power_port;
		uint8_t data_port;
		float last_humidity_reading;
};
