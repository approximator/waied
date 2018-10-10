#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import json
import random
import logging
import http.server
import socketserver
from datetime import datetime, timedelta

logger = logging.getLogger("WaiedTestDataServer")

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(ROOT_DIR, 'testData')

PORT = 8012


class Handler(http.server.SimpleHTTPRequestHandler):

    timedeltas = (timedelta(days=-0), timedelta(days=-1), timedelta(days=-5), timedelta(days=-10))

    def __init__(self, req, client_addr, server):
        http.server.SimpleHTTPRequestHandler.__init__(self, req, client_addr, server)

    def do_GET(self):
        logger.debug('Request path: {}'.format(self.path))
        if self.path.startswith('/search'):
            logger.info('Returning predefined set of issues')
            data = open(os.path.join(DATA_DIR, 'task_list.json'), 'r').read().encode()
        elif self.path.endswith('/worklog'):
            data = self.get_worklog(self.path.split('/')[-2])
        else:
            logger.error('Unknown request')
            data = '{"error": "unknown request"}'

        self.send_response(200)
        self.send_header("Content-type", "application/json")
        self.send_header("Content-length", len(data))
        self.end_headers()
        self.wfile.write(data)

    @staticmethod
    def isoformat(dt):
        return dt.isoformat().split(".")[0] + '.000+0000'

    def get_worklog(self, worklog_id):
        logger.info('Looking for worklog {}'.format(worklog_id))
        file_name = os.path.join(DATA_DIR, 'worklog_{}.json'.format(worklog_id))
        data = json.load(open(file_name, 'r'))
        for worklog in data['worklogs']:
            n = datetime.now() + random.choice(Handler.timedeltas)
            worklog['started'] = Handler.isoformat(n)
            worklog['updated'] = Handler.isoformat(n)
        return json.dumps(data).encode()


logging.basicConfig(level='INFO')
httpd = socketserver.TCPServer(('localhost', PORT), Handler)
logger.info('Serving at localhost:{}'.format(PORT))
httpd.serve_forever()
