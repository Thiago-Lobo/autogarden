#pragma once

#include "arduino_wrapper.h"

#define RELAY_MAXIMUM_TIMED_STEP_MILLIS 10000 

class Relay
{
	public:
		Relay(uint8_t relay_port);
		~Relay();
		void set_state(bool state);
		bool toggle();
		bool update();
		void timed_step(bool state, uint32_t millis);
	private:
		bool state;
		uint8_t port;
		uint32_t timed_step_reference;
		uint32_t timed_step_delta_millis;
		void refresh_output();
};
