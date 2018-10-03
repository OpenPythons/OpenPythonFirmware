# ubinascii
"binary/ASCII conversions"


def hexlify(data: str, sep: str = None) -> bytes:
    "Convert binary data to hexadecimal representation. Returns bytes string."
    pass


def unhexlify(data: str) -> bytes:
    "Convert hexadecimal data to binary representation. Returns bytes string. (i.e. inverse of hexlify)"
    pass


def a2b_base64(data: str) -> bytes:
    "Decode base64-encoded data, ignoring invalid characters in the input. Conforms to RFC 2045 s.6.8. Returns a bytes object."
    pass


def b2a_base64(data: str) -> bytes:
    "Encode binary data in base64 format, as in RFC 3548. Returns the encoded data followed by a newline character, as a bytes object."
    pass
