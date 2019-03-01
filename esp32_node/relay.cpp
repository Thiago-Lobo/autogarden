#include "relay.h"

Relay::Relay(uint8_t relay_port)
{
	port = relay_port;
	state = false;
	my_make_output(port);
	refresh_output();
	timed_step_reference = 0;
	timed_step_delta_millis = 0;
}

Relay::~Relay()
{
}

void Relay::refresh_output()
{
	my_output_mode(port, (!state) ? ON : OFF);
}

void Relay::set_state(bool new_state)
{
	state = new_state;
	refresh_output();
}

bool Relay::toggle()
{
	state = !state;
	refresh_output();

	return state;
}

void Relay::timed_step(bool new_state, uint32_t delta_millis)
{
	if (delta_millis == 0)
	{
		return;
	}

	set_state(new_state);
	timed_step_delta_millis = (delta_millis > RELAY_MAXIMUM_TIMED_STEP_MILLIS) ? RELAY_MAXIMUM_TIMED_STEP_MILLIS : delta_millis;
	timed_step_reference = my_time_millis();
}

bool Relay::update()
{
	if ((timed_step_delta_millis > 0) && (my_time_millis() >= timed_step_reference + timed_step_delta_millis))
	{
		toggle();
		timed_step_delta_millis = 0;

		return true;
	}

	return false;
}
