#pragma once

#include <SPI.h>
#include <EEPROM.h>
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include "rf_message.h"
#include "arduino_wrapper.h"

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

// # print header.from_node
// # print header.to_node
// # print header.type
// # print header.id
// # print header.next_id
// # print header.reserved

// # if header.type == MeshMaster.RF_MESSAGE_ID_RESPOND_TEMPERATURE:
// # 	result = struct.unpack('<f', payload)
// # else:
// # 	result = struct.unpack('<L', payload)

// # result = struct.unpack('<f', payload)
// # result = struct.unpack('<L', payload)
// # print 'Result: {}'.format(result)

// # if not self.mesh.write(struct.pack("L", 5000), MeshMaster.RF_MESSAGE_ID_START_PUMP, 1):
// # if not self.mesh.write(struct.pack("f", 2.98746), MeshMaster.RF_MESSAGE_ID_REQUEST_HUMIDITY, 1):
// # if not self.mesh.write(struct.pack("f", 2.98746), MeshMaster.RF_MESSAGE_ID_REQUEST_LUMINOSITY, 1):
// # if not self.mesh.write(struct.pack("f", 2.98746), MeshMaster.RF_MESSAGE_ID_REQUEST_TEMPERATURE, 1):
// # if not self.mesh.write(struct.pack("L", 4532), MeshMaster.RF_MESSAGE_ID_REQUEST_SANITY, 1):