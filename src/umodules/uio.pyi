# uio
"input/output streams"
import typing


# noinspection PyShadowingBuiltins
def open(name, mode='r', **kwargs) -> typing.IO:
    "Open a file. Builtin open() function is aliased to this function. All ports (which provide access to file system) are required to support mode parameter, but support for other arguments vary by port."
    pass


# noinspection PyAbstractClass
class FileIO(typing.TextIO):
    "This is type of a file open in binary mode, e.g. using open(name, 'rb'). You should not instantiate this class directly."

    def __init__(self, address: str, path: str, mode: str):
        pass


# noinspection PyAbstractClass
class TextIOWrapper(typing.BinaryIO):
    "This is type of a file open in text mode, e.g. using open(name, 'rt'). You should not instantiate this class directly."
    pass


# noinspection PyAbstractClass
class StringIO(typing.TextIO):
    "In-memory file-like objects for input/output. StringIO is used for text-mode I/O (similar to a normal file opened with “t” modifier). Initial contents of file-like objects can be specified with string parameter (should be normal string for StringIO or bytes object for BytesIO). All the usual file methods like read(), write(), seek(), flush(), close() are available on these objects, and additionally, a following method:"
    pass


# noinspection PyAbstractClass
class BytesIO(typing.BinaryIO):
    "In-memory file-like objects for input/output. BytesIO is used for binary-mode I/O (similar to a normal file opened with “b” modifier). Initial contents of file-like objects can be specified with string parameter (should be normal string for StringIO or bytes object for BytesIO). All the usual file methods like read(), write(), seek(), flush(), close() are available on these objects, and additionally, a following method:"
    pass

    def getvalue(self) -> bytes:
        pass


def resource_stream():
    pass
