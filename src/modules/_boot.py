import machine
from handler import print_handler, signal_handler, input_handler
machine.interrupt_hook(signal_handler)
machine.input_hook(input_handler)
machine.print_hook(print_handler)

__all__ = ["print_handler", "signal_handler"]
del __all__
