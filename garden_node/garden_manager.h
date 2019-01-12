#pragma once

#include "rf_message.h"
#include "mesh_node.h"
#include "relay.h"
#include "temperature_sensor.h"
#include "soil_humidity_sensor.h"
#include "luminosity_sensor.h"

#define GARDEN_MANAGER_PUMP_RELAY_PIN 0x6
#define GARDEN_MANAGER_TEMPERATURE_SENSOR_DATA_PIN 0x2
#define GARDEN_MANAGER_HUMIDITY_SENSOR_POWER_PIN A3
#define GARDEN_MANAGER_HUMIDITY_SENSOR_DATA_PIN A0
#define GARDEN_MANAGER_LUMINOSITY_SENSOR_POWER_PIN A5
#define GARDEN_MANAGER_LUMINOSITY_SENSOR_DATA_PIN A6
#define GARDEN_MANAGER_MESH_NODE_ID 1
#define GARDEN_MANAGER_MESH_NODE_CE_PIN 9
#define GARDEN_MANAGER_MESH_NODE_CS_PIN 10

class GardenManager
{
	public:
		GardenManager();
		~GardenManager();
		void update();
		void handle_incoming_message();
	private:
		Relay* pump;
		TemperatureSensor* temperature_sensor;
		SoilHumiditySensor* humidity_sensor;
		LuminositySensor* luminosity_sensor;
		MeshNode* mesh_node;
		uint8_t* message_buffer;
		uint8_t* header_buffer;
		
		uint32_t timer = 0;
};
