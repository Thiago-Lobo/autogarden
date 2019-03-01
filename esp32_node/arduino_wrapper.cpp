#include "arduino_wrapper.h"

#ifdef ESP32
static my_pwm_channel my_pwm_channels[MY_PWM_MAX_CHANNELS];
static uint8_t pwm_channel_counter = 0;

void debug_pwm_channels()
{
  my_print("Next channel: ");my_println(pwm_channel_counter);
  my_println();
  
  for (uint8_t i = 0; i < MY_PWM_MAX_CHANNELS; i++)
  {
    my_print("Channel: ");my_println(i);
    my_print("> Frequency: ");my_println(my_pwm_channels[i].frequency);
    my_print("> Resolution: ");my_println(my_pwm_channels[i].resolution);
    my_print("> Pin: ");my_println(my_pwm_channels[i].pin);
    my_print("> Duty cycle: ");my_println(my_pwm_channels[i].duty_cycle);
  }
}

void initialize_pwm(uint8_t pin, uint8_t resolution, uint32_t frequency)
{
	ledcSetup(pwm_channel_counter, frequency, resolution);
	ledcAttachPin(pin, pwm_channel_counter);

	my_pwm_channels[pwm_channel_counter].frequency = frequency;
	my_pwm_channels[pwm_channel_counter].resolution = resolution;
	my_pwm_channels[pwm_channel_counter].pin = pin;
	my_pwm_channels[pwm_channel_counter].duty_cycle = 0;

	pwm_channel_counter = (pwm_channel_counter + 1) % MY_PWM_MAX_CHANNELS;
  Serial.println(pwm_channel_counter);
}

void analogWrite(uint8_t pin, uint32_t duty_cycle)
{
	for (uint8_t i = 0; i < MY_PWM_MAX_CHANNELS; i++)
	{
//    Serial.print("Trying ");Serial.print(my_pwm_channels[i].pin);Serial.print(" as ");Serial.println(pin);
		if (my_pwm_channels[i].pin == pin)
		{
// Workaround for the ESP32 bug (https://github.com/espressif/arduino-esp32/issues/1909)
      if (i > 7 && duty_cycle == 0)
      {
        duty_cycle = 1;
      }
			my_pwm_channels[i].duty_cycle = duty_cycle;
			ledcWrite(i, duty_cycle);
      break;
		}
	}
}
#elif defined(ARDUINO_NANO)
void initialize_pwm(uint8_t pin, uint8_t resolution, uint32_t frequency)
{
	my_make_output(pin);
}
#endif

bool is_string_integer(String str)
{
	for (uint8_t i = 0; i < str.length(); i++)
	{
		if (!(isDigit(str.charAt(i)))) 
		{
			return false;
		}
	}

	return true;
}
