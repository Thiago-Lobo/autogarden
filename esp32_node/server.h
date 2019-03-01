#pragma once

#include "arduino_wrapper.h"

#include "WiFi.h"
#include "WebServer.h"

#include "luminosity_sensor.h"
#include "soil_humidity_sensor.h"
#include "temperature_sensor.h"
#include "relay.h"

#define SOIL_HUMIDITY_SENSOR_POWER_PORT 22
#define SOIL_HUMIDITY_SENSOR_DATA_PORT 34
#define SOIL_HUMIDITY_SENSOR_OBSERVER_PORT 35

#define LUMINOSITY_SENSOR_POWER_PORT 23
#define LUMINOSITY_SENSOR_DATA_PORT 36
#define LUMINOSITY_SENSOR_OBSERVER_PORT 39

#define TEMPERATURE_SENSOR_POWER_PORT 33
#define TEMPERATURE_SENSOR_DATA_PORT 32

#define MESSAGE_QUEUE_SIZE 100

#define RELAY_PORT 15

#define SSID "Brigadeiro de Queijo"
#define PASSWORD "carandache"

#define WEBSERVER_PORT 80
#define WEBSERVER_GET_TEMPERATURE_ENDPOINT "/get_temperature"
#define WEBSERVER_GET_LUMINOSITY_ENDPOINT "/get_luminosity"
#define WEBSERVER_GET_HUMIDITY_ENDPOINT "/get_humidity"
#define WEBSERVER_GET_ALL_SENSORS_ENDPOINT "/get_all_sensors"
#define WEBSERVER_ENABLE_PUMP_ENDPOINT "/enable_pump"

class GardenNodeServer
{
	public:
		GardenNodeServer();
		~GardenNodeServer();
		void serve();
		static GardenNodeServer* instances[1];

		// Endpoint handler wrappers
		static void handle_get_temperature_wrapper();
		static void handle_get_luminosity_wrapper();
		static void handle_get_humidity_wrapper();
		static void handle_get_all_sensors_wrapper();
		static void handle_enable_pump_wrapper();
		static void handle_not_found_wrapper();
	private:
		WebServer* server;
		
		LuminositySensor* luminosity_sensor;
		SoilHumiditySensor* soil_humidity_sensor;
		TemperatureSensor* temperature_sensor;
		Relay* pump;

		void initialize_wifi();
		void initialize_sensors();
		void initialize_server();

		// Enpoint handlers
		void handle_get_temperature();
		void handle_get_luminosity();
		void handle_get_humidity();
		void handle_get_all_sensors();
		void handle_enable_pump();
		void handle_not_found();
};
