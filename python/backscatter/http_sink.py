#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 gr-backscatter author.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#


from functools import partial
import pmt
from gnuradio import gr
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
import threading
import json


class pdu_queue:
    def __init__(self):
        self.pdus = []
        self.expected = 0

    def clear(self):
        self.pdus.clear()

    def push(self, pdu):
        self.pdus.append(pdu)


class http_handler(BaseHTTPRequestHandler):
    def __init__(self, queue: pdu_queue, event: threading.Event, *args, **kwargs):
        self.queue: pdu_queue = queue
        self.event = event
        super().__init__(*args, **kwargs)

    def do_GET(self):
        parsed_path = urlparse(self.path)
        if parsed_path.path == "/pdus":
            query = parse_qs(parsed_path.query)
            n = 10
            if "n" in query:
                n = int(query["n"][0])
            self.queue.clear()
            self.queue.expected = n
            self.event.clear()
            self.event.wait()
            self.send_response(200)
            self.send_header("Content-type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps(self.queue.pdus).encode())
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write("Not found".encode())

    def log_message(*args, **kargs):
        pass


class http_sink(gr.sync_block):
    def __init__(self, host="127.0.0.1", port=52001):
        gr.sync_block.__init__(self, name="http_sink", in_sig=[], out_sig=None)
        self.httpd = None
        self.event = threading.Event()
        self.queue = pdu_queue()
        self.host = host
        self.port = port
        self.message_port_register_in(pmt.intern("pdus"))
        self.set_msg_handler(pmt.intern("pdus"), self.handle_pdu)

    def run_server(self):
        server_address = (self.host, self.port)
        handler = partial(http_handler, self.queue, self.event)
        self.httpd = HTTPServer(server_address, handler)
        self.httpd.serve_forever()

    def start(self):
        threading.Thread(target=self.run_server).start()

    def stop(self):
        self.httpd.shutdown()
        self.httpd = None

    def handle_pdu(self, msg):
        if self.queue.expected > 0:
            self.queue.push(pmt.to_python(msg)[1].tolist())
            self.queue.expected -= 1
        else:
            self.event.set()
