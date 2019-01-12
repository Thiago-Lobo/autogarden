#!/usr/local/bin/python

import sys
import os
import sqlite3 as sqlite
import random
import time

# DB_PATH = 'local.db'
DB_PATH = 'file::memory:?cache=shared'

DB_STATEMENT_CREATE_TABLE = '''
	create table if not exists "test"
	(
		id integer primary key autoincrement,
		producer_id integer,
		payload number,
		acked bool,
		consumer_id integer
	) 
'''

DB_STATEMENT_INSERT_ENTRY = '''
	insert into test (producer_id, payload, acked, consumer_id) values (?,?,?,?)
'''

PRODUCER_SLEEP_PERIOD = 0.1

def sqlite_connect_and_intialize():
	db = sqlite.connect(DB_PATH, timeout = 0.1, uri=True)
	cursor = db.cursor()
	cursor.execute(DB_STATEMENT_CREATE_TABLE)
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

def write_entry(producer_id, consumer_count, db):
	sqlite_safe_execute(db, DB_STATEMENT_INSERT_ENTRY, (producer_id, random.random() * 100, False, int(random.random() * consumer_count)))
	db.commit()

def get_consumer_count():
	return sys.argv[1]

def get_producer_id():
	return sys.argv[2]

def main():
	producer_id = int(get_producer_id())
	consumer_count = int(get_consumer_count())


	db = sqlite_connect_and_intialize()
	
	then = time.time()
	count = 0

	while 1:
		time.sleep(PRODUCER_SLEEP_PERIOD)
		
		count += 1
		write_entry(producer_id, consumer_count, db)	
		
		if time.time() - then > 1:
			print '{0} writes per second!'.format(count)
			count = 0
			then = time.time()


	db.close()

if __name__ == '__main__':
	main()