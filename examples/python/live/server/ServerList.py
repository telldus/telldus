# -*- coding: utf-8 -*-

import httplib
import xml.parsers.expat

class ServerList():

	def __init__(self):
		self.list = []
		self.retrieveServerList()

	def popServer(self):
		if (self.list == []):
			self.retrieveServerList()

		if (self.list == []):
			return False

		return self.list.pop(0)


	def retrieveServerList(self):
		conn = httplib.HTTPConnection("api.telldus.com:80")
		conn.request('GET', "/server/assign?protocolVersion=2")
		response = conn.getresponse()

		p = xml.parsers.expat.ParserCreate()

		p.StartElementHandler = self._startElement
		p.Parse(response.read())

	def _startElement(self, name, attrs):
		if (name == 'server'):
			self.list.append(attrs)
