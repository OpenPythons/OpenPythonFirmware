from usystem import set_stdin_char

from system import debug
from system import syscall, components, parse


class Component:
    def __init__(self, address, type):
        self.address = address
        self.type = type

    def __getattr__(self, name):
        return lambda *args: syscall(self.address, name, *args)

    def __dir__(self):
        return dir(object) + ["address", "type"]

    def __repr__(self):
        return "Component<{0}:{1}>".format(self.type, self.address)


devices = {value: Component(key, value) for key, value in components().items()}
com = devices["computer"]
gpu = devices["gpu"]
screen = devices["screen"]
fs = devices["filesystem"]


def test():
    gpu.bind(screen.address)


def _bootstrap():
    pass  # print(syscall("hello", "world", 32))


class Monitor:
    def __init__(self):
        self.width = 50
        self.height = 16
        self.widthPos = 1
        self.heightPos = 1

    def scroll(self):
        gpu.copy(1, 2, self.width, self.height, 0, - 1)
        gpu.fill(1, self.height, self.width, 1, " ")

    def putChar(self, char):
        if char == "\r":
            self.widthPos = 1
        if char == "\n":
            self.heightPos += 1

        if self.widthPos > self.width:
            self.widthPos = 1
            self.heightPos += 1

        if self.heightPos > self.height:
            self.scroll()
            self.heightPos = self.height

        if char == "\r" or char == "\n":
            return

        gpu.set(self.widthPos, self.heightPos, char)
        self.widthPos += 1


gpu.bind(screen.address)
monitor = Monitor()


def print_handler(buf):
    try:
        for char in buf:
            monitor.putChar(char)
    except BaseException as e:
        debug("exc", type(e).__name__ + ":" + str(e))
        debug("trace", dir(e))


def signal_handler(signal_buf):
    try:
        signal = parse(signal_buf)
        if signal is None:
            debug("signal is empty")
            return

        name = signal["name"]  # type: str
        args = signal["args"]  # type: tuple
        if name == "key_down" and len(args) >= 4:
            _, char, _, _, *_ = args
            set_stdin_char(int(char))
        elif name == "key_up" and len(args) >= 4:
            pass
        elif name == "component_added" and len(args) >= 2:
            debug("signal", name, args, len(args))
            address, device_type, *_ = args
            devices[device_type] = Component(address, device_type)
        else:
            debug("signal", name, args, len(args))
    except BaseException as e:
        debug("exc", type(e).__name__ + ":" + str(e))


debug("done")

# set_print_hook(print_hook)

try:
    _bootstrap()
finally:
    del _bootstrap
