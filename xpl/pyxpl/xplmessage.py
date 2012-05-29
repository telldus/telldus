###########################################################################
#    Copyright (C) 2009 by Magnus Ahlberg 
#    <magnus.ahlberg@svart-katt.se>
#
# Copyright: See COPYING file that comes with this distribution
#
###########################################################################

class XPLMessage:
    """A class for managing xPL messages"""
    ID_UNDEFINED, ID_XPLCMND, ID_XPLSTAT, ID_XPLTRIG = range(4)
    __body = {}
    __hop = 0
    __source = ""
    __target = ""
    __identifier = ID_UNDEFINED
    __identifierString = ("-", "xpl-cmnd", "xpl-stat", "xpl-trig")
    __messageSchemaIdentifier = ""

    def __init__(self, identifier, messageSchemaIdentifier):
        self.__identifier = identifier
        self.__messageSchemaIdentifier = messageSchemaIdentifier

    def addBodyItem(self, key, value):
        self.__body[key] = value

    def bodyItem(self, key):
        return self.__body[key]

    def createMessageFromString(message):
        pass

    def setSource(self, value):
        self.__source = value

    def setTarget(self, value):
        self.__target = value

    def toString(self):
        if self.__identifier == self.ID_UNDEFINED:
            return ""

        messageStringList = []
        messageStringList.append(self.__identifierString[self.__identifier] + "\n")
        messageStringList.append("{\n")
        messageStringList.append("hop=" + str(self.__hop) + "\n")
        messageStringList.append("source=" + self.__source + "\n")
        messageStringList.append("target=" + self.__target + "\n")
        messageStringList.append("}\n")
        messageStringList.append(self.__messageSchemaIdentifier + "\n")
        messageStringList.append("{\n")
        for key, value in self.__body.iteritems():
            messageStringList.append(key + "=" + value + "\n")
        messageStringList.append("}\n")

        return "".join(messageStringList)