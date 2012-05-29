###########################################################################
#    Copyright (C) 2009 by Magnus Ahlberg
#    <magnus.ahlberg@svart-katt.se>
#
# Copyright: See COPYING file that comes with this distribution
#
###########################################################################

from xpldevice import *

from socket import *

class XPLInstance:
    """Class to handle an xPL session"""
    MODE_UNDEFINED, MODE_CLIENT, MODE_DISCONNECTED, MODE_HUB = range(4)

    __thisDevice = XPLDevice()
    __devices = ()
    __mode = MODE_UNDEFINED

    def __init__(self, device):
        self.__thisDevice = device

    def __init__(self, vendor, deviceName):
        device = XPLDevice(vendor, deviceName)
        self.__init__(device)

    def attached(self):
        pass

    def devices(self):
        return devices

    def operationMode(self):
        return operationMode

    def sendMessage(self, message):
        pass

    def sendMessage(self, message, device):
        pass

    def shutdown(self):
        pass

    def bindToPort(self):
        pass
