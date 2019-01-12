from RF24 import *
from RF24Network import *
from RF24Mesh import *

import struct

class MeshMaster:
	MESH_MASTER_CE_PIN = 17
	MESH_MASTER_CS_PIN = 0
	MESH_MASTER_NODE_ID = 0

	def __init__(self):
		self.radio = RF24(MeshMaster.MESH_MASTER_CE_PIN, MeshMaster.MESH_MASTER_CS_PIN)
		self.network = RF24Network(self.radio)
		self.mesh = RF24Mesh(self.radio, self.network)
		self.mesh.setNodeID(MeshMaster.MESH_MASTER_NODE_ID)
		self.mesh.begin()
		self.radio.setPALevel(RF24_PA_MAX)
		self.radio.printDetails()

	def send(self, data, id):
		# while not self.mesh.write(data, id, 1):
			# self.update()
			# print 'Send fail!'
		self.mesh.write(data, id, 1)

	def receive(self):
		if self.network.available():
			header, payload = self.network.read(10)
			return header.type, payload

		return None, None

	def update(self):
		self.mesh.update()
		self.mesh.DHCP()

	def long_to_bytes(self, long_data):
		return struct.pack("L", long_data)

	def bytes_to_long(self, payload):
		return struct.unpack('<L', payload)[0]

	def float_to_bytes(self, float_data):
		return struct.pack("f", float_data)

	def bytes_to_float(self, payload):
		return struct.unpack('<f', payload)[0]
		
def main():
	mesh_master = MeshMaster()

	count = 0

	while 1:
		mesh_master.update()

		if count % 200000 == 0:
			mesh_master.send(mesh_master.long_to_bytes(5000), 77)

		header, payload = mesh_master.receive()

		if header:
			print header

		count += 1

if __name__ == '__main__':
	main()
