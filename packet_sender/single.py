from sender.sender import *
from sender.signal import *
from sender.server import *

import sys

if len(sys.argv) == 3:
	signalId = int(sys.argv[1])
	signalRaw = int(sys.argv[2])
	signal = Signal(signalId)
	server = Server("127.0.0.1", 3334)
	sender = Sender(server)	
	signal.update(signalRaw)
	sender.send(signal)
else:
    print "Wrong argument type. Usage: python {} <signalId> <signalRaw>".format(sys.argv[0])	
	
	
	
	
	
