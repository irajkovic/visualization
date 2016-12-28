from sender.sender import *
from sender.signal import *
from sender.server import *

import sys
import psutil

server = Server("127.0.0.1", 3334)
sender = Sender(server)	
signal = Signal(0)

if len(sys.argv) == 2:
	signalId = int(sys.argv[1])
	signal = Signal(signalId)
	
else:
    print "Wrong argument type. Usage: python {} <signalId>".format(sys.argv[0])	
    sys.exit()
	

while True:
    cpuUsage = int(psutil.cpu_percent(interval=0.1))
    signal.update(cpuUsage)
    sender.send(signal)


