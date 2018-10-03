# sys
"system specific functions"

# def exit(retval=0): pass
# Terminate current program with a given exit code. Underlyingly, this function raise as SystemExit exception. If an argument is given, its value given as an argument to SystemExit.
from collections import namedtuple


def print_exception(exc, file=None):
    "Print exception with a traceback to a file-like object file (or sys.stdout by default)."
    pass


argv = []  # A mutable list of arguments the current program was started with.
byteorder = "little"  # The byte order of the system ("little" or "big").
implementation = namedtuple("implementation", "name, version")  # (name="micropython", version=(0, 0, 1))
maxsize = 2147483647  # Maximum value which a native integer type can hold on the current platform, or maximum value representable by MicroPython integer type, if it’s smaller than platform max value  (that is the case for MicroPython ports without long int support).
modules = {}  # Dictionary of loaded modules. On some ports, it may not include builtin modules.
path = []  # A mutable list of directories to search for imported modules.
# platform = "?"  # The platform that MicroPython is running on. For OS/RTOS ports, this is usually an identifier of the OS, e.g. "linux". For baremetal ports it is an identifier of a board, e.g. "pyboard" for the original MicroPython reference board. It thus can be used to distinguish one board from another. If you need to check whether your program runs on MicroPython (vs other Python implementation), use sys.implementation instead.
stderr = None  # Standard error stream.
stdin = None  # Standard input stream.
stdout = None  # Standard output stream.
version = "3.4.0"  # Python language version that this implementation conforms to, as a string.
version_info = (3, 4, 0)  # Python language version that this implementation conforms to, as a tuple of ints.
