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

API_KEY_FILE = 'telegram_api.key'
GARDEN_MASTER_CONFIG_FILE = 'config.json'

TELEGRAM_COMMAND_SET_HUMIDITY_PERIOD = 'set_humidity_period'

TRACKING_PERIOD_MINUTES = 15

####################################################
## Handler Callbacks
####################################################

def unknown(bot, update):
	bot.send_message(chat_id=update.message.chat_id, text='Unknown command.', reply_markup=None)

def set_humidty_period(bot, update, job_queue, args):
	period = args[0]
	
	# check for valid value in seconds

	bot.send_message(chat_id=update.message.chat_id, text='Now tracking flights in URL "{0}" for prices less than {1}.'.format(url, price_threshold))

####################################################
## Initializers
####################################################

def initialize_bot():
	k = open(API_KEY_FILE, 'r')

	updater = Updater(token = k.readlines()[0].strip())
	dispatcher = updater.dispatcher
	queue = updater.job_queue
	
	dispatcher.add_handler(CommandHandler(TELEGRAM_COMMAND_SET_HUMIDITY_PERIOD, set_humidty_period, pass_args=True, pass_job_queue=True))
	dispatcher.add_handler(MessageHandler(Filters.command, unknown))
		
	updater.start_polling()
	updater.idle()

def main():
	initialize_bot()

if __name__ == '__main__':
	main()
