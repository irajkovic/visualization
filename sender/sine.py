from sender.sender import *
from sender.signal import *
from sender.server import *
import math
from time import sleep

import sys

server = Server("127.0.0.1", 3334)
sender = Sender(server)	
signal = Signal(0)

max = 100

if len(sys.argv) == 3:
	signalId = int(sys.argv[1])
	signal = Signal(signalId)
	max = int(sys.argv[2])
	
else:
    print "Wrong argument type. Usage: python {} <signalId>".format(sys.argv[0])	
    sys.exit()
	
i = 0
max = max / 2

while True:
    signal.update(int(math.sin(math.radians(i))*max + max))
    sender.send(signal)
    i=i+4
    sleep(0.1)
    if i >= 360:
        i = 0


