import micropython
from usystem import signal as pop_signal, set_stdin_char

from component import Component, Monitor, devices
from system import debug

devices = devices()
gpu = devices["gpu"]
screen = devices["screen"]

gpu.bind(screen.address)
monitor = Monitor(gpu)
gpu.fill(1, 1, monitor.widthSize, monitor.heightSize, " ")


def print_handler(buf):
    try:
        for char in buf:
            monitor.putChar(char)
    except BaseException as e:
        debug("exc", type(e).__name__ + ":" + str(e))
        debug("trace", dir(e))


def signal_handler(_):
    micropython.schedule(signal_handle, None)


def signal_handle(_):
    DEBUG = False
    while True:
        try:
            signal = pop_signal()
            if not signal:
                break

            name, args = signal # type: str, tuple
            if name == "key_down" and len(args) >= 4:
                # when redirectKeyEvent set then never called
                _, char, _, _, *_ = args
                set_stdin_char(int(char))
            elif name == "clipboard" and len(args) >= 3:
                _, value, _, *_ = args
                for char in value:
                    set_stdin_char(ord(char))
            elif name == "key_up" and len(args) >= 4:
                pass
            elif name == "component_added" and len(args) >= 2:
                if DEBUG:
                    debug("signal", name, args, len(args))
                address, device_type, *_ = args
                devices[device_type] = Component(address, device_type)
            else:
                if DEBUG:
                    debug("signal", name, args, len(args))
        except BaseException as e:
            debug("exc", type(e).__name__ + ":" + str(e))
