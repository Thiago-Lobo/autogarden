#ifndef ARDUINO_WRAPPER_H
#define ARDUINO_WRAPPER_H

#include "Arduino.h"

#define ESP32
// #define ARDUINO_NANO

#define MY_DEFAULT_SERIAL Serial
#define my_print MY_DEFAULT_SERIAL.print
#define my_println MY_DEFAULT_SERIAL.println

#ifdef ESP32
	#define MY_ADC_RESOLUTION 12
	#define MY_ADC_MAX_VOLTAGE 3.3f
	
	// analogWrite custom implementation for the ESP32 :)

	#define MY_PWM_DEFAULT_FREQUENCY 980
	#define MY_PWM_DEFAULT_RESOLUTION 8

	#define MY_PWM_MAX_CHANNELS 16

	struct my_pwm_channel {
		uint32_t frequency;
		uint8_t resolution;
		uint8_t pin;
		uint32_t duty_cycle;
	};
	
	extern void initialize_pwm(uint8_t pin, uint8_t resolution = MY_PWM_DEFAULT_RESOLUTION, uint32_t frequency = MY_PWM_DEFAULT_FREQUENCY);
	extern void analogWrite(uint8_t pin, uint32_t duty_cycle);
  extern void debug_pwm_channels();
#elif defined(ARDUINO_NANO)
	#define MY_ADC_RESOLUTION 10
	#define MY_ADC_MAX_VOLTAGE 5.0f

	extern void initialize_pwm(uint8_t pin, uint8_t resolution = 0, uint32_t frequency = 0);
#endif

extern bool is_string_integer(String str);

#define my_pwm(x, y) (analogWrite((x), (y)))
#define my_time_micros() (micros())
#define my_time_millis() (millis())
#define my_time() (my_time_micros() / 1000000.0f)
#define my_delay(x) (delay((x)))

#define ADC_TO_VOLT(x) ((float) (x) / ((float) (1 << MY_ADC_RESOLUTION)) * MY_ADC_MAX_VOLTAGE)
#define my_adc(x) (analogRead((x)))
#define my_adc_volts(x) (ADC_TO_VOLT(my_adc((x))))

#define ON HIGH
#define OFF LOW
#define my_make_output(x) (pinMode((x), OUTPUT))
#define my_make_input(x) (pinMode((x), INPUT))
#define my_output_mode(x, y) (digitalWrite((x), (y)))
#define my_digital_read(x) (digitalRead(x))

#define MY_PI 		3.14159265358979323f
#define MY_TAU 		(2.0f * MY_PI)
#define MY_EULER 	2.71828182845904523f

#define MY_DEG_FACTOR (180.0f / MY_PI)
#define MY_RAD_FACTOR (MY_PI / 180.0f)
#define MY_DEG(x) ((x) * MY_DEG_FACTOR)
#define MY_RAD(x) ((x) * MY_RAD_FACTOR)

#define MY_SQR(x) ((x) * (x))
#define MY_POW(a, b) (powf(a, b))
#define MY_MIN(a, b) ((a) < (b) ? (a) : (b))
#define MY_MAX(a, b) ((a) > (b) ? (a) : (b))
#define MY_ABS(x) ((x) >= 0.0f ? (x) : -(x))
#define MY_SQRT(x) (sqrt(x))
#endif
