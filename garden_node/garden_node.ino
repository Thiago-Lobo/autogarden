#include "Arduino.h"

#include "arduino_wrapper.h"
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
