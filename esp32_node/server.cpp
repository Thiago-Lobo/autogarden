#include "server.h"

GardenNodeServer* GardenNodeServer::instances[1] = { NULL };

GardenNodeServer::GardenNodeServer()
{
	instances[0] = this;

	initialize_sensors();
	initialize_wifi();
	initialize_server();
}

GardenNodeServer::~GardenNodeServer()
{

}

void GardenNodeServer::initialize_wifi()
{
	my_print("Connecting to SSID: "); 
	my_println(SSID);

	WiFi.begin(SSID, PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		my_delay(1000);
		my_print(".");
	}
	
	my_println("WiFi successfully connected.");
	my_print("Got IP: ");
	my_println(WiFi.localIP());
}

void GardenNodeServer::initialize_sensors()
{
	temperature_sensor = new TemperatureSensor(TEMPERATURE_SENSOR_POWER_PORT, TEMPERATURE_SENSOR_DATA_PORT);
	luminosity_sensor = new LuminositySensor(LUMINOSITY_SENSOR_POWER_PORT, LUMINOSITY_SENSOR_DATA_PORT, LUMINOSITY_SENSOR_OBSERVER_PORT);
	soil_humidity_sensor = new SoilHumiditySensor(SOIL_HUMIDITY_SENSOR_POWER_PORT, SOIL_HUMIDITY_SENSOR_DATA_PORT, SOIL_HUMIDITY_SENSOR_OBSERVER_PORT);
	pump = new Relay(RELAY_PORT);
}

void GardenNodeServer::initialize_server()
{
	server = new WebServer(WEBSERVER_PORT); 

	server->on(WEBSERVER_GET_TEMPERATURE_ENDPOINT, handle_get_temperature_wrapper);
	server->on(WEBSERVER_GET_LUMINOSITY_ENDPOINT, handle_get_luminosity_wrapper);
	server->on(WEBSERVER_GET_HUMIDITY_ENDPOINT, handle_get_humidity_wrapper);
	server->on(WEBSERVER_GET_ALL_SENSORS_ENDPOINT, handle_get_all_sensors_wrapper);
	server->on(WEBSERVER_ENABLE_PUMP_ENDPOINT, handle_enable_pump_wrapper);
	server->onNotFound(handle_not_found_wrapper);

	server->begin();
	my_println("HTTP Server successfully initialized.");
}

void GardenNodeServer::serve()
{
	while (true)
	{
		pump->update();
		server->handleClient();
	}
}

void GardenNodeServer::handle_get_temperature_wrapper()
{
	if (GardenNodeServer::instances[0] != NULL)
	{
		GardenNodeServer::instances[0]->handle_get_temperature();
	}
}

void GardenNodeServer::handle_get_temperature()
{
	temperature_sensor->update();
	server->send(200, "text/html", String(temperature_sensor->get_temperature()));
}

void GardenNodeServer::handle_get_luminosity_wrapper()
{
	if (GardenNodeServer::instances[0] != NULL)
	{
		GardenNodeServer::instances[0]->handle_get_luminosity();
	}
}

void GardenNodeServer::handle_get_luminosity()
{
	luminosity_sensor->update();
	server->send(200, "text/html", String(luminosity_sensor->get_luminosity()));
}	

void GardenNodeServer::handle_get_humidity_wrapper()
{
	if (GardenNodeServer::instances[0] != NULL)
	{
		GardenNodeServer::instances[0]->handle_get_humidity();
	}
}

void GardenNodeServer::handle_get_humidity()
{
	soil_humidity_sensor->update();
	server->send(200, "text/html", String(soil_humidity_sensor->get_humidity()));
}

void GardenNodeServer::handle_get_all_sensors_wrapper()
{
	if (GardenNodeServer::instances[0] != NULL)
	{
		GardenNodeServer::instances[0]->handle_get_all_sensors();
	}
}

void GardenNodeServer::handle_get_all_sensors()
{
	temperature_sensor->update();
	luminosity_sensor->update();
	soil_humidity_sensor->update();

	server->send(200, "text/html", String(String(temperature_sensor->get_temperature()) + String(";") + String(luminosity_sensor->get_luminosity()) + String(";") + String(soil_humidity_sensor->get_humidity())));
}

void GardenNodeServer::handle_enable_pump_wrapper()
{
	if (GardenNodeServer::instances[0] != NULL)
	{
		GardenNodeServer::instances[0]->handle_enable_pump();
	}
}

void GardenNodeServer::handle_enable_pump()
{
	if (server->hasArg("period"))
	{
		if (is_string_integer(server->arg("period")))
		{
			pump->timed_step(true, (uint32_t) server->arg("period").toInt());
		}
		else
		{
			server->send(400, "text/html", "Invalid period");
			return;
		}
	}
	else
	{
		pump->timed_step(true, 7500);
	}
	
	server->send(204, "text/html", "");
}

void GardenNodeServer::handle_not_found_wrapper()
{
	if (GardenNodeServer::instances[0] != NULL)
	{
		GardenNodeServer::instances[0]->handle_not_found();
	}
}

void GardenNodeServer::handle_not_found()
{
	server->send(404, "text/html", "Not found");
}
