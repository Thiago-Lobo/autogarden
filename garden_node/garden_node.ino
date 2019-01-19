#include "Arduino.h"
#include "SPI.h"
#include "EEPROM.h"
#include "arduino_wrapper.h"
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <printf.h>
#include "garden_manager.h"

GardenManager* manager;

void setup() 
{
  	Serial.begin(115200);
    printf_begin();
    manager = new GardenManager();
}

void loop()
{
	manager->update();
}
