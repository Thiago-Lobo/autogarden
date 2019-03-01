#pragma once

#include "arduino_wrapper.h"

#define LUMINOSITY_SENSOR_AVERAGE_SIZE 500
#define LUMINOSITY_SENSOR_VOLTAGE_DIVIDER_R1 10000.0f

class LuminositySensor
{
	public:
		LuminositySensor(uint8_t power_port, uint8_t data_port, uint8_t sensor_observer_port = 0xFF);
		~LuminositySensor();
		float get_luminosity();
		void update();
	private:
		uint8_t power_port;
		uint8_t data_port;
		uint8_t observer_port;
		float last_luminosity_reading;
};
