#pragma once

#include <SPI.h>
#include <EEPROM.h>
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include "rf_message.h"

#define AG_XCEIVER_NODE_ID 1
#define AG_XCEIVER_CE_PIN 9
#define AG_XCEIVER_CS_PIN 10

struct payload_t {
	unsigned long ms;
	unsigned long counter;
};

class MeshNode
{
	public:
		MeshNode(uint8_t node_id, uint8_t ce_pin, uint8_t cs_pin);
		~MeshNode();
		void update();
		void send(void* data, uint8_t id);
		bool receive(uint8_t* buffer, uint8_t* header);
	private:
		RF24* radio;
		RF24Network* network;
		RF24Mesh* mesh;
};
