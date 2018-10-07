import machine
from machine import debug, signal as pop_signal

from component import Component, Monitor, devices

devices = devices()
gpu = devices["gpu"]
screen = devices["screen"]

monitor = Monitor(gpu)
gpu.bind(screen.address)
gpu.fill(1, 1, monitor.widthSize, monitor.heightSize, " ")

buf = []


@machine.hook_input
def input_handler():
    while not buf:
        signal_handler()

    return int(buf.pop(0))


@machine.hook_print
def print_handler(buf):
    try:
        monitor.put(buf)
    except BaseException as e:
        debug("print_handler exc =? %s: %s" % (type(e).__name__, e))


@machine.hook_signal
def signal_handler(ticks):
    DEBUG = False
    try:
        signal = pop_signal(ticks)
        if signal is None:
            return

        # debug(ujson.dumps(signal) + '\n')
        name, args = signal  # type: str, tuple
        if name == "key_down" and len(args) >= 4:
            # when redirectKeyEvent set then never called
            _, char, _, _, *_ = args
            buf.append(char)
        elif name == "clipboard" and len(args) >= 3:
            _, value, _, *_ = args
            # for char in value:
            #     set_stdin_char(ord(char))
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
        debug("signal_handler exc => %s: %s" % (type(e).__name__, e))
