import time

class Signal(object):
    
    def __init__(self, id):
        self.id = id
        self.raw = min
        self.timestamp = time.time()
        self.packageCnt = 0
        
        
    def update(self, raw):
        self.raw = raw
        self.timestamp = time.time()
        ++self.packageCnt
        
        
        
