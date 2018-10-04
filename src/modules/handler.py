import micropython
from machine import signal as pop_signal, set_stdin_char, debug
import ujson

from component import Component, Monitor, devices

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
        debug("exc%s: %s" % (type(e).__name__ , e))


def signal_handler(_):
    micropython.schedule(signal_handle, None)


def signal_handle(_):
    DEBUG = False
    while True:
        try:
            # TODO: sleeping is possble with ticks=n
            #       sleeping with some ticks but signal available then resume computer and give signal
            #       ExecutionResult.Sleep(inf) => when signal is available then return signal
            signal = pop_signal()
            # debug("check signal " + repr(signal))
            if not signal:
                break

            debug(ujson.dumps(signal) + '\n')

            name, args = signal  # type: str, tuple
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
            debug("exc%s: %s" % (type(e).__name__ , e))
