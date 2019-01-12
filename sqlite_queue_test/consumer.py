#!/usr/local/bin/python

import sys
import os
import sqlite3 as sqlite
import random
import time

# DB_PATH = 'local.db'
DB_PATH = 'file::memory:?cache=shared'

DB_STATEMENT_CONSUME_ENTRY = '''
	update test set acked = 1 where consumer_id = {consumer_id} and acked = 0
'''

DB_STATEMENT_COUNT_ENTRIES = '''
	select count(*) from test where consumer_id = {consumer_id} and acked = 0
'''

CONSUMER_SLEEP_PERIOD = 0.5

def sqlite_connect_and_intialize():
	db = sqlite.connect(DB_PATH, timeout = 0.1, uri=True)
	return db

def sqlite_safe_execute(db, statement, args=False):
	cursor = db.cursor()

	count = 0

	while 1:
		try:
			if args:
				cursor.execute(statement, args)
			else:
				cursor.execute(statement)
			break
		except sqlite.OperationalError:
			count += 1
			print 'Failed: {0}'.format(count)

	return cursor

def get_consumer_id():
	return sys.argv[1]

def read(db, consumer_id):
	cursor = sqlite_safe_execute(db, DB_STATEMENT_CONSUME_ENTRY.format(consumer_id = consumer_id))
	
	if cursor.rowcount > 0:
		cursor.rowcount

	db.commit()
	
def count(db, consumer_id):
	cursor = sqlite_safe_execute(db, DB_STATEMENT_COUNT_ENTRIES.format(consumer_id = consumer_id))

	non_acked_entries = cursor.fetchone()[0]

	if non_acked_entries > 0:
		print 'Non-acked entries: {0}'.format(non_acked_entries)

def main():
	consumer_id = int(get_consumer_id())
	
	db = sqlite_connect_and_intialize()
	
	while 1:
		time.sleep(CONSUMER_SLEEP_PERIOD)
		count(db, consumer_id)
		read(db, consumer_id)

	db.close()

if __name__ == '__main__':
	main()