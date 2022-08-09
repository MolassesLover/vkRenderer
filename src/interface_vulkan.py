from ctypes import cdll

lib = cdll.LoadLibrary("./libTEMPORARYNAME.so")

class Application(object):
    def __init__(self):
        self.obj = lib.Application_new()
    def Run(self):
        lib.Application_Run(self.obj)

if __name__ == "__main__":
    app = Application()
    
    app.run()