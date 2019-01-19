from mesh_master import MeshMaster
import sqlite3
import json
from datetime import datetime
import time
import logging

class GardenMaster:
	GARDEN_MASTER_CONFIG_FILE = 'config.json'
	RF_MESSAGE_ID_START_PUMP = 77
	RF_MESSAGE_ID_PUMP_FINISHED = 78
	RF_MESSAGE_ID_REQUEST_HUMIDITY = 79
	RF_MESSAGE_ID_RESPOND_HUMIDITY = 80
	RF_MESSAGE_ID_REQUEST_LUMINOSITY = 81
	RF_MESSAGE_ID_RESPOND_LUMINOSITY = 82
	RF_MESSAGE_ID_REQUEST_TEMPERATURE = 83
	RF_MESSAGE_ID_RESPOND_TEMPERATURE = 84
	RF_MESSAGE_ID_REQUEST_SANITY = 85
	RF_MESSAGE_ID_RESPOND_SANITY = 86

	def __init__(self):
		self.initialize_logging()
		self.logger = logging.getLogger(__name__)

		self.logger.info('Initializing RF24 Mesh module')
		self.mesh_master = MeshMaster()
		
		self.logger.info('Reading config file')
		file = open(GardenMaster.GARDEN_MASTER_CONFIG_FILE, 'r')
		self.config = json.load(file)
		file.close()
		self.logger.info('Loaded configs: {0}'.format(json.dumps(self.config)))
		
		self.humidity_timer = 0
		self.luminosity_timer = 0
		self.temperature_timer = 0
		self.pump_flags = [False for i in range(len(self.config['pump_times']))]

		self.current_day = datetime.now().day
		self.logger.info('Current day is {0}'.format(self.current_day))
	
	def initialize_logging(self):
		logging.basicConfig(
			filename='garden_master.log',
			level=logging.DEBUG,
			format='%(asctime)s.%(msecs)03d [%(name)s] %(levelname)-7s %(funcName)s - %(message)s', 
			datefmt='%Y-%m-%d %H:%M:%S'
		)

	def update_pump_flags(self):
		now = datetime.now()

		if now.day != self.current_day:
			self.logger.info('Updating pump flags: {0}'.format(json.dumps(self.pump_flags)))
			self.pump_flags = [False for i in range(len(self.config['pump_times']))]
			self.logger.info('Updated pump flags: {0}'.format(json.dumps(self.pump_flags)))
			self.current_day = now.day

	def receive(self):
		header, payload = self.mesh_master.receive()

		if header:
			self.logger.info('Message received - header: {0}'.format(header))
			if header == GardenMaster.RF_MESSAGE_ID_PUMP_FINISHED:
				self.logger.info('Pump finished running')
			elif header == GardenMaster.RF_MESSAGE_ID_RESPOND_HUMIDITY:
				humidity = self.mesh_master.bytes_to_float(payload)
				self.logger.info('Humidity: {0}'.format(humidity))
			elif header == GardenMaster.RF_MESSAGE_ID_RESPOND_LUMINOSITY:
				luminosity = self.mesh_master.bytes_to_float(payload)
				self.logger.info('Luminosity: {0}'.format(luminosity))
			elif header == GardenMaster.RF_MESSAGE_ID_RESPOND_TEMPERATURE:
				temperature = self.mesh_master.bytes_to_float(payload)
				self.logger.info('Temperature: {0}'.format(temperature))
			elif header == GardenMaster.RF_MESSAGE_ID_RESPOND_SANITY:
				a=1

	def update(self):
		self.mesh_master.update()
		self.update_pump_flags()
		self.receive()

		for i in range(len(self.config['pump_times'])):
			reference = datetime.strptime(self.config['pump_times'][i], '%H:%M')
			now = reference.replace(hour = datetime.now().hour, minute = datetime.now().minute)
			if now == reference and not self.pump_flags[i]:
				self.logger.info('Pump flags: {0}'.format(json.dumps(self.pump_flags)))
				self.pump_flags[i] = True
				self.logger.info('Updated pump flags: {0}'.format(json.dumps(self.pump_flags)))
				self.logger.info('Sending START_PUMP command related to configured time {0}'.format(self.config['pump_times'][i]))
				self.mesh_master.send(self.mesh_master.long_to_bytes(8500), GardenMaster.RF_MESSAGE_ID_START_PUMP)

		# Fetch humidity
		if (time.time() - self.humidity_timer >= self.config['humidity_period']):
			self.logger.info('Sending periodic REQUEST_HUMIDITY command')
			self.mesh_master.send(self.mesh_master.long_to_bytes(0), GardenMaster.RF_MESSAGE_ID_REQUEST_HUMIDITY)
			self.humidity_timer = time.time()

		# Fetch luminosity
		if (time.time() - self.luminosity_timer >= self.config['luminosity_period']):
			self.logger.info('Sending periodic REQUEST_LUMINOSITY command')
			self.mesh_master.send(self.mesh_master.long_to_bytes(0), GardenMaster.RF_MESSAGE_ID_REQUEST_LUMINOSITY)
			self.luminosity_timer = time.time()

		# Fetch temperature
		if (time.time() - self.temperature_timer >= self.config['temperature_period']):
			self.logger.info('Sending periodic REQUEST_TEMPERATURE command')
			self.mesh_master.send(self.mesh_master.long_to_bytes(0), GardenMaster.RF_MESSAGE_ID_REQUEST_TEMPERATURE)
			self.temperature_timer = time.time()
		
def main():
	garden_master = GardenMaster()

	while 1:
		garden_master.update()
                time.sleep(0.2)

if __name__ == '__main__':
	main()
