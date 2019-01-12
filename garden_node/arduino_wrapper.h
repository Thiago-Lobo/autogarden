#pragma once

#define DEFAULT_SERIAL Serial

#define ag_print DEFAULT_SERIAL.print
#define ag_println DEFAULT_SERIAL.println

#define ag_pwm(x, y) (analogWrite((x), (y)))
#define ag_time_micros() (micros())
#define ag_time_millis() (millis())
#define ag_time() (ag_time_micros() / 1000000.0f)
#define ag_delay(x) (delay((x)))

#define ADC_RESOLUTION 10
#define ADC_MAX_VOLTAGE 5.0f
#define ADC_TO_VOLT(x) ((float) (x) / ((float) (1 << ADC_RESOLUTION)) * ADC_MAX_VOLTAGE)
#define ag_adc(x) (analogRead((x)))
#define ag_adc_volts(x) (ADC_TO_VOLT(ag_adc((x))))

#define ON HIGH
#define OFF LOW
#define ag_make_output(x) (pinMode((x), OUTPUT))
#define ag_make_input(x) (pinMode((x), INPUT))
#define ag_output_mode(x, y) (digitalWrite((x), (y)))
#define ag_digital_read(x) (digitalRead(x))

void ag_float_to_bytes(float value, byte bytes_array[]) {
    union {
        float float_variable;
        byte temp_array[4];
    } u;

    u.float_variable = value;

    memcpy(bytes_array, u.temp_array, 4);
}

#define AG_PI 		3.14159265358979323f
#define AG_TAU 		(2.0f * AG_PI)
#define AG_EULER 	2.71828182845904523f

#define AG_DEG_FACTOR (180.0f / AG_PI)
#define AG_RAD_FACTOR (AG_PI / 180.0f)
#define AG_DEG(x) ((x) * AG_DEG_FACTOR)
#define AG_RAD(x) ((x) * AG_RAD_FACTOR)

#define AG_SQR(x) ((x) * (x))
#define AG_POW(a, b) (powf(a, b))
#define AG_MIN(a, b) ((a) < (b) ? (a) : (b))
#define AG_MAX(a, b) ((a) > (b) ? (a) : (b))
#define AG_ABS(x) ((x) >= 0.0f ? (x) : -(x))
#define AG_SQRT(x) (sqrt(x))
