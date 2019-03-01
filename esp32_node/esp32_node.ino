#include "arduino_wrapper.h"

#include "WiFi.h"
#include "WebServer.h"
#include "FS.h"

#include "server.h"

GardenNodeServer* server;

void setup() 
{
	Serial.begin(115200);
	server = new GardenNodeServer();
}

void loop()
{
	server->serve();
}
