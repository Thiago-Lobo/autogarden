#include "garden_manager.h"

GardenManager::GardenManager()
{
	pump = new Relay(0x6);
	temperature_sensor = new TemperatureSensor(0x2);
	humidity_sensor = new SoilHumiditySensor(GARDEN_MANAGER_HUMIDITY_SENSOR_POWER_PIN, GARDEN_MANAGER_HUMIDITY_SENSOR_DATA_PIN);
	luminosity_sensor = new LuminositySensor(GARDEN_MANAGER_LUMINOSITY_SENSOR_POWER_PIN, GARDEN_MANAGER_LUMINOSITY_SENSOR_DATA_PIN);
	mesh_node = new MeshNode(GARDEN_MANAGER_MESH_NODE_ID, GARDEN_MANAGER_MESH_NODE_CE_PIN, GARDEN_MANAGER_MESH_NODE_CS_PIN);
	
	message_buffer = malloc(RF_MESSAGE_PAYLOAD_LENGTH);
	header_buffer = malloc(sizeof(uint8_t));
}

GardenManager::~GardenManager()
{
}

void GardenManager::update()
{
	// if timed_step has switched off
	if (pump->update())
	{
		// message_buffer sent as dummy mandatory value
		mesh_node->send(message_buffer, RF_MESSAGE_ID_PUMP_FINISHED);
	}
	
	mesh_node->update();

	if (mesh_node->receive(message_buffer, header_buffer))
	{
		handle_incoming_message();
	}
}

void GardenManager::handle_incoming_message()
{
	if (*header_buffer == RF_MESSAGE_ID_START_PUMP)
	{
		ag_println("Incoming RF_MESSAGE_ID_START_PUMP!");
		uint32_t step_size;
		memcpy(&step_size, message_buffer, RF_MESSAGE_PAYLOAD_LENGTH);
		ag_print("Got step length: ");ag_println(step_size);
		pump->timed_step(true, step_size);
	}
	else if (*header_buffer == RF_MESSAGE_ID_REQUEST_HUMIDITY)
	{
		ag_println("Incoming RF_MESSAGE_ID_REQUEST_HUMIDITY!");
		humidity_sensor->update();
		float humidity = humidity_sensor->get_humidity();
		ag_print("Got humidity: ");ag_println(humidity);
		mesh_node->send(&humidity, RF_MESSAGE_ID_RESPOND_HUMIDITY);
	}
	else if (*header_buffer == RF_MESSAGE_ID_REQUEST_LUMINOSITY)
	{
		ag_println("Incoming RF_MESSAGE_ID_REQUEST_LUMINOSITY!");
		luminosity_sensor->update();
		float luminosity = luminosity_sensor->get_luminosity();
		ag_print("Got luminosity: ");ag_println(luminosity);
		mesh_node->send(&luminosity, RF_MESSAGE_ID_RESPOND_LUMINOSITY);
	}
	else if (*header_buffer == RF_MESSAGE_ID_REQUEST_TEMPERATURE)
	{
		ag_println("Incoming RF_MESSAGE_ID_REQUEST_TEMPERATURE!");
		temperature_sensor->update();
		float temperature = temperature_sensor->get_temperature();
		ag_print("Got temperature: ");ag_println(temperature);
		mesh_node->send(&temperature, RF_MESSAGE_ID_RESPOND_TEMPERATURE);
	}
	else if (*header_buffer == RF_MESSAGE_ID_REQUEST_SANITY)
	{
		ag_println("Incoming RF_MESSAGE_ID_REQUEST_SANITY!");
		uint32_t number;
		memcpy(&number, message_buffer, RF_MESSAGE_PAYLOAD_LENGTH);
		ag_print("Got step number: ");ag_println(number);
		number += 1;
		mesh_node->send(&number, RF_MESSAGE_ID_RESPOND_SANITY);
	}
	else
	{
		ag_println("Other!");
	}
}
