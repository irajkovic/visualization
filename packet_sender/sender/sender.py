import signal

import socket
import time
import math
import sys
import struct
import os

class Sender(object):
    
    def __init__(self, server):
        self.server = server
        self.sock = socket.socket(socket.AF_INET, # Internet
             socket.SOCK_DGRAM) # UDP
        
    def getRawBytes(self, value):
        return struct.unpack("4b", struct.pack('>I', value))

        
    def preparePackage(self, signal):
        
        signalIdBytes = self.getRawBytes(signal.id) 
        signalPackageCntBytes = self.getRawBytes(signal.packageCnt)
        
        
        package = [ signalIdBytes[2], \
		  signalIdBytes[3], \
		  signalPackageCntBytes[2], \
		  signalPackageCntBytes[3], \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x05, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00, \
		  0x00 ]
        
        # timestamp
        package_timestmap_ind = 4
        timestamp = int(signal.timestamp * 100)
        for i in range(0, 8):
            package[package_timestmap_ind + i] = (timestamp >> ((7 - i) * 8)) & 0xFF
        
        
        #value 
        value = signal.raw
        package_value_ind = 12               
        for i in range(0, 8):
            package[package_value_ind + i] = (value >> ((7 - i) * 8)) & 0xFF
            
        # checksum
        package[20] = 0x00
        checksum = 0x00;
        for x in package:
            checksum ^= x
        package[20] = checksum
        
        message = ''.join(chr(x) for x in package) 
        
        return message



    def send(self, signal):
        message = self.preparePackage(signal)
        self.sock.sendto(message, (self.server.ip, self.server.port))
        
        print "Sending value {}".format(signal.raw)

