#!/usr/bin/python

import sys
import os
import re
import json
import time
import traceback
from datetime import datetime, timedelta
from telegram.ext import Updater, CommandHandler, MessageHandler, Filters, CallbackQueryHandler
from telegram import ReplyKeyboardMarkup, InlineKeyboardButton, InlineKeyboardMarkup
import logging
import requests

GARDEN_NODE_URL = 'http://esp32/'

GET_TEMPERATURE_ENDPOINT = 'get_temperature'
GET_LUMINOSITY_ENDPOINT = 'get_luminosity'
GET_HUMIDITY_ENDPOINT = 'get_humidity'
GET_ALL_SENSORS_ENDPOINT = 'get_all_sensors'

ENABLE_PUMP_ENDPOINT = 'enable_pump'
ENABLE_PUMP_PARAM_PERIOD = 'period'
DEFAULT_ENABLE_PUMP_PERIOD = 8000

HTTP_REQUEST_TIMEOUT = 5

API_KEY_FILE = 'telegram_api.key'

TELEGRAM_COMMAND_TEST = 'test'
TELEGRAM_COMMAND_GET_TEMPERATURE = 'get_temperature'
TELEGRAM_COMMAND_GET_LUMINOSITY = 'get_luminosity'
TELEGRAM_COMMAND_GET_HUMIDITY = 'get_humidity'
TELEGRAM_COMMAND_GET_ALL_SENSORS = 'get_all_sensors'
TELEGRAM_COMMAND_ENABLE_PUMP = 'enable_pump'

SCHEDULED_WATERING = [
	"06:00:00",
	"15:00:00"
]

logger = None

def string_to_timestamp(string):
	return datetime.strptime(string, '%H:%M:%S')
	# return datetime.strptime(string, '%Y-%m-%d %H:%M:%S')

def initialize_logging():
	logging.basicConfig(
		filename='garden_bot.log',
		level=logging.INFO,
		format='%(asctime)s.%(msecs)03d [%(name)s] %(levelname)-7s %(funcName)s - %(message)s', 
		datefmt='%Y-%m-%d %H:%M:%S'
	)

def create_dummy_telegram_update():
	chat = telegram.Chat(58880229, '')
	user = telegram.User(58880229, '', False, username='thiago_lobo')
	message = telegram.Message(0, user, datetime.now(), chat, )
	update = telegram.Update(0, message)
	return update

def get_all_sensors_callback(bot, job):
	try:
		result = requests.get(GARDEN_NODE_URL + GET_ALL_SENSORS_ENDPOINT, timeout=HTTP_REQUEST_TIMEOUT)

		http_status = result.status_code

		if http_status in [200, 204]:
			values = result.text.split(';')
			bot.send_message(chat_id=58880229, text='Current temperature is: {0} C\nCurrent luminosity is: {1}\nCurrent humidity is: {2} %'.format(values[0], values[1], values[2]))
		else:
			bot.send_message(chat_id=58880229, text='HTTP request failed: {0}'.format(http_status))
	except Exception as e:
		bot.send_message(chat_id=58880229, text=str(e))

def periodic_pump_enabler(bot, job):
	bot.send_message(chat_id=58880229, text='Will start daily plant watering job')

	period = DEFAULT_ENABLE_PUMP_PERIOD

	try:
		result = requests.get(GARDEN_NODE_URL + ENABLE_PUMP_ENDPOINT, params={ENABLE_PUMP_PARAM_PERIOD: period}, timeout=HTTP_REQUEST_TIMEOUT)

		http_status = result.status_code

		if http_status in [200, 204]:
			bot.send_message(chat_id=58880229, text='Watering plants for {0} milliseconds'.format(period))
			time.sleep(int(period / 1000))
			get_all_sensors_callback(bot, job)
		else:
			bot.send_message(chat_id=58880229, text='HTTP request failed: {0}'.format(http_status))
	except Exception as e:
		bot.send_message(chat_id=58880229, text=str(e))

def authenticate_telegram_user(user_id, chat_id, username):
	if user_id == 58880229 and chat_id == 58880229 and username == 'thiago_lobo':
		return True

	return False

####################################################
## Handler Callbacks
####################################################

def unknown(bot, update):
	bot.send_message(chat_id=update.message.chat_id, text='Unknown command.', reply_markup=None)

def get_temperature(bot, update):
	if authenticate_telegram_user(update.message.from_user.id, update.message.chat_id, update.message.from_user.username):
		try:
			result = requests.get(GARDEN_NODE_URL + GET_TEMPERATURE_ENDPOINT, timeout=HTTP_REQUEST_TIMEOUT)

			http_status = result.status_code

			if http_status in [200, 204]:
				bot.send_message(chat_id=update.message.chat_id, text='Current temperature is: {0} C'.format(result.text))
			else:
				bot.send_message(chat_id=update.message.chat_id, text='HTTP request failed: {0}'.format(http_status))
		except Exception as e:
			bot.send_message(chat_id=update.message.chat_id, text=str(e))

def get_humidity(bot, update):
	if authenticate_telegram_user(update.message.from_user.id, update.message.chat_id, update.message.from_user.username):
		try:
			result = requests.get(GARDEN_NODE_URL + GET_HUMIDITY_ENDPOINT, timeout=HTTP_REQUEST_TIMEOUT)

			http_status = result.status_code

			if http_status in [200, 204]:
				bot.send_message(chat_id=update.message.chat_id, text='Current humidity is: {0} %'.format(result.text))
			else:
				bot.send_message(chat_id=update.message.chat_id, text='HTTP request failed: {0}'.format(http_status))
		except Exception as e:
			bot.send_message(chat_id=update.message.chat_id, text=str(e))

def get_luminosity(bot, update):
	if authenticate_telegram_user(update.message.from_user.id, update.message.chat_id, update.message.from_user.username):
		try:
			result = requests.get(GARDEN_NODE_URL + GET_LUMINOSITY_ENDPOINT, timeout=HTTP_REQUEST_TIMEOUT)

			http_status = result.status_code

			if http_status in [200, 204]:
				bot.send_message(chat_id=update.message.chat_id, text='Current luminosity is: {0}'.format(result.text))
			else:
				bot.send_message(chat_id=update.message.chat_id, text='HTTP request failed: {0}'.format(http_status))
		except Exception as e:
			bot.send_message(chat_id=update.message.chat_id, text=str(e))

def get_all_sensors(bot, update):
	if authenticate_telegram_user(update.message.from_user.id, update.message.chat_id, update.message.from_user.username):
		try:
			result = requests.get(GARDEN_NODE_URL + GET_ALL_SENSORS_ENDPOINT, timeout=HTTP_REQUEST_TIMEOUT)

			http_status = result.status_code

			if http_status in [200, 204]:
				values = result.text.split(';')
				bot.send_message(chat_id=update.message.chat_id, text='Current temperature is: {0} C\nCurrent luminosity is: {1}\nCurrent humidity is: {2} %'.format(values[0], values[1], values[2]))
			else:
				bot.send_message(chat_id=update.message.chat_id, text='HTTP request failed: {0}'.format(http_status))
		except Exception as e:
			bot.send_message(chat_id=update.message.chat_id, text=str(e))

def enable_pump(bot, update, job_queue, args):
	if authenticate_telegram_user(update.message.from_user.id, update.message.chat_id, update.message.from_user.username):
		period = DEFAULT_ENABLE_PUMP_PERIOD

		if len(args) != 0:
			try:
				period = int(args[0])
				if period < 0:
					bot.send_message(chat_id=update.message.chat_id, text='Invalid period')
					return
			except:
				bot.send_message(chat_id=update.message.chat_id, text='Invalid period')
				return

		try:
			result = requests.get(GARDEN_NODE_URL + ENABLE_PUMP_ENDPOINT, params={ENABLE_PUMP_PARAM_PERIOD: period}, timeout=HTTP_REQUEST_TIMEOUT)

			http_status = result.status_code

			if http_status in [200, 204]:
				bot.send_message(chat_id=update.message.chat_id, text='Watering plants for {0} milliseconds'.format(period))
				job_queue.run_once(lambda bot, update: get_all_sensors(bot, update), (int(period / 1000) + 5))
				job_queue.run_once(lambda bot, update: get_all_sensors(bot, update), (int(period / 1000) + 65))
			else:
				bot.send_message(chat_id=update.message.chat_id, text='HTTP request failed: {0}'.format(http_status))
		except Exception as e:
			bot.send_message(chat_id=update.message.chat_id, text=str(e))

def test(bot, update, job_queue, args):
	bot.send_message(chat_id=update.message.chat_id, text='Hello cruel world.')
	bot.send_message(chat_id=update.message.chat_id, text=update.message.from_user.id)
	bot.send_message(chat_id=update.message.chat_id, text=update.message.chat_id)
	bot.send_message(chat_id=update.message.chat_id, text=update.message.from_user.username)

####################################################
## Initializers
####################################################

def initialize_bot():
	k = open(API_KEY_FILE, 'r')

	updater = Updater(token = k.readlines()[0].strip())
	dispatcher = updater.dispatcher
	queue = updater.job_queue
	
	dispatcher.add_handler(CommandHandler(TELEGRAM_COMMAND_GET_TEMPERATURE, get_temperature))
	dispatcher.add_handler(CommandHandler(TELEGRAM_COMMAND_GET_HUMIDITY, get_humidity))
	dispatcher.add_handler(CommandHandler(TELEGRAM_COMMAND_GET_LUMINOSITY, get_luminosity))
	dispatcher.add_handler(CommandHandler(TELEGRAM_COMMAND_GET_ALL_SENSORS, get_all_sensors))
	dispatcher.add_handler(CommandHandler(TELEGRAM_COMMAND_ENABLE_PUMP, enable_pump, pass_args=True, pass_job_queue=True))
	# dispatcher.add_handler(CommandHandler(TELEGRAM_COMMAND_TEST, test, pass_args=True, pass_job_queue=True))
	dispatcher.add_handler(MessageHandler(Filters.command, unknown))
	
	for formatted_timestamp in SCHEDULED_WATERING:
		queue.run_daily(periodic_pump_enabler, string_to_timestamp(formatted_timestamp).time())

	updater.start_polling()
	updater.idle()

def main():
	global logger
	logger = logging.getLogger(__name__)
	initialize_logging()
	initialize_bot()

if __name__ == '__main__':
	main()
