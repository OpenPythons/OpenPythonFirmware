# noinspection PyUnresolvedReferences
from handler import print_handler, signal_handler
from system import debug

try:
    from component import Component

    devices = Component.list()

    fs = devices["filesystem"]
    import uos
    from vfs import FileSystem

    uos.mount(FileSystem(fs.address), '/')
except BaseException as e:
    msg = "{}: {}".format(type(e).__name__, str(e))
    debug("exc", msg)
    print("exc", msg)
    del msg

devices = Component.list()
com = devices["computer"]
gpu = devices["gpu"]
screen = devices["screen"]
fs = devices["filesystem"]
