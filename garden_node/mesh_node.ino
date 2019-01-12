#include "mesh_node.h"

MeshNode::MeshNode(uint8_t node_id, uint8_t ce_pin, uint8_t cs_pin)
{
	radio = new RF24(ce_pin, cs_pin);
	network = new RF24Network(*radio);
	mesh = new RF24Mesh(*radio, *network);

	mesh->setNodeID(AG_XCEIVER_NODE_ID);
	Serial.println(F("Connecting to the mesh..."));
	while (!mesh->begin());
	radio->printDetails();
}

MeshNode::~MeshNode()
{

}

void MeshNode::update()
{
	mesh->update();
}

void MeshNode::send(void* data, uint8_t id)
{
	while (!mesh->write(data, id, RF_MESSAGE_PAYLOAD_LENGTH))
	{
		ag_println("Write failed. Checking connection.");
		if (!mesh->checkConnection()) 
		{
			ag_println("Bad connection. Renewing address.");
			mesh->renewAddress();
		}
	}
}

bool MeshNode::receive(uint8_t* buffer, uint8_t* header)
{
	if (network->available())
	{
		ag_println("Message received!");
		RF24NetworkHeader rf24_header;
		network->read(rf24_header, buffer, RF_MESSAGE_PAYLOAD_LENGTH);
		*header = rf24_header.type;
		ag_print("Header: ");ag_println(*header);
		return true;
	}

	return false;
}
