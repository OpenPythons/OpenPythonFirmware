# uctypes
"access binary data in a structured way"
from typing import Union, Type

LITTLE_ENDIAN: int  # Layout type for a little-endian packed structure. (Packed means that every field occupies exactly as many bytes as defined in the descriptor, i.e. the alignment is 1).
BIG_ENDIAN: int  # Layout type for a big-endian packed structure.
NATIVE: int  # Layout type for a native structure - with data endianness and alignment conforming to the ABI of the system on which MicroPython runs.


class struct:
    "Instantiate a “foreign data structure” object based on structure address in memory, descriptor (encoded as a dictionary), and layout type (see below)."

    def __init__(self, addr, descriptor, layout_type=NATIVE):
        pass


def sizeof(struct: Union[struct, Type[struct]]) -> int:
    "Return size of data structure in bytes. Argument can be either structure class or specific instantiated structure object (or its aggregate field)."
    pass


def addressof(obj) -> int:
    "Return address of an object. Argument should be bytes, bytearray or other object supporting buffer protocol (and address of this buffer is what actually returned)."
    pass


def bytes_at(addr, size) -> bytes:
    "Capture memory at the given address and size as bytes object. As bytes object is immutable, memory is actually duplicated and copied into bytes object, so if memory contents change later, created object retains original value."
    pass


def bytearray_at(addr, size) -> bytearray:
    "Capture memory at the given address and size as bytearray object. Unlike bytes_at() function above, memory is captured by reference, so it can be both written too, and you will access current value at the given memory address."
    pass
