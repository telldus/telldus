# -*- coding: utf-8 -*-

import ctypes

class TelldusCore():

	def __init__(self):
		self.supportedMethods = 0
		self.lib = ctypes.cdll.LoadLibrary('libtelldus-core.so')
		self.list = []
		for i in range(self.lib.tdGetNumberOfDevices()):
			id = self.lib.tdGetDeviceId(i)
			device = {'id': id}
			namePtr = self.lib.tdGetName(id)
			device['name'] = ctypes.c_char_p(namePtr).value
			self.lib.tdReleaseString(namePtr)
			stateValuePtr = self.lib.tdLastSentValue(id)
			device['stateValue'] = ctypes.c_char_p(stateValuePtr).value
			self.lib.tdReleaseString(stateValuePtr)
			self.list.append(device)

	def getList(self):
		return self.list

	def setSupportedMethods(self, supportedMethods):
		if (self.supportedMethods == supportedMethods):
			return

		self.supportedMethods = supportedMethods
		for device in self.list:
			device['methods'] = self.lib.tdMethods(device['id'], supportedMethods)
			device['state'] = self.lib.tdLastSentCommand(device['id'], supportedMethods)

	def turnoff(self, id):
		print "Turning off: %i" % id
		self.lib.tdTurnOff(id)

	def turnon(self, id):
		print "Turning on: %i" % id
		self.lib.tdTurnOn(id)
