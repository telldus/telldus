# -*- coding: utf-8 -*-

import socket, ssl
import time, os

from configobj import ConfigObj

from ServerList import *
from TelldusCore import *
from LiveMessage import *

class Client():
	def __init__(self):
		self.publicKey = ''
		self.privateKey = ''
		self.hashMethod = 'sha1'
		self.pongTimer = 0
		self.supportedMethods = 0
		self.tellduscore = TelldusCore()
		self.serverList = ServerList()

		self.configPath = os.environ['HOME'] + '/.config/Telldus'
		self.configFilename = 'TelldusLive.conf'
		self.config = ConfigObj(self.configPath + '/' + self.configFilename)
		self.connect(self.serverList.popServer())

	def __del__(self):
		try:
			os.makedirs(self.configPath)
		except:
			pass
		self.config.write()

	def connect(self, server):
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket = ssl.wrap_socket(s, ssl_version=ssl.PROTOCOL_TLSv1, ca_certs="/etc/ssl/certs/ca-certificates.crt",cert_reqs=ssl.CERT_REQUIRED)
		self.socket.settimeout(5)
		self.socket.connect((server['address'], int(server['port'])))

		uuid = ''
		try:
			uuid = self.config['uuid']
		except:
			pass

		msg = LiveMessage('Register')
		msg.append({
			'key': self.publicKey,
			'uuid': uuid,
			'hash': self.hashMethod
		})
		msg.append({
			'protocol': 2,
			'version': '1',
			'os': 'linux',
			'os-version': 'unknown'
		})

		self.socket.write(self.signedMessage(msg))
		self.pongTimer = time.time()
		while(1):
			try:
				resp = self.socket.read(1024)
			except ssl.SSLError:
				# Timeout, try again after some maintenance
				if (time.time() - self.pongTimer >= 360):  # No pong received
					print("No pong received, disconnecting")
					break

				continue
			if (resp == ''):
				print("no response")
				break

			envelope = LiveMessage.fromByteArray(resp)
			if (not envelope.verifySignature(self.hashMethod, self.privateKey)):
				print "Signature failed"
				continue

			self.pongTimer = time.time()
			self.handleMessage(LiveMessage.fromByteArray(envelope.argument(0).stringVal))

	def handleCommand(self, args):
		if (args['action'].stringVal == 'turnon'):
			self.tellduscore.turnon(args['id'].intVal)
		elif (args['action'].stringVal == 'turnoff'):
			self.tellduscore.turnoff(args['id'].intVal)
		else:
			return

		if ('ACK' in args):
			#Respond to ack
			msg = LiveMessage("ACK")
			msg.append(args['ACK'].intVal)
			self.socket.write(self.signedMessage(msg))

	def handleMessage(self, message):
		if (message.name() == "notregistered"):
			params = message.argument(0).dictVal
			self.config['uuid'] = params['uuid'].stringVal
			self.config['activationUrl'] = params['url'].stringVal
			print "This client isn't activated, please activate it using this url:\n%s" % params['url'].stringVal
			return

		if (message.name() == "registered"):
			params = message.argument(0).dictVal
			self.supportedMethods = params['supportedMethods'].intVal
			self.tellduscore.setSupportedMethods(self.supportedMethods)
			self.sendDevicesReport()
			return

		if (message.name() == "command"):
			self.handleCommand(message.argument(0).dictVal)
			return

		if (message.name() == "pong"):
			return

		print "Did not understand: %s" % message.toByteArray()


	def sendDevicesReport(self):
		msg = LiveMessage("DevicesReport")
		msg.append(self.tellduscore.getList())
		self.socket.write(self.signedMessage(msg))

	def signedMessage(self, message):
		return message.toSignedMessage(self.hashMethod, self.privateKey)
