import ujson
import usystem
from usystem import debug as _debug

__all__ = ["invoke", "signal", "components", "methods", "debug", "ExternalException"]


class ExternalException(Exception):
    pass


def invoke(*req):
    res = usystem.invoke(*req)
    result, error = res
    if error is not None:
        raise ExternalException(error)

    if result is None or len(result) == 0:
        return None
    elif len(result) == 1:
        return result[0]
    else:
        return tuple(result)


def signal():
    return usystem.signal()


def components():
    return usystem.components()


def methods(address_or_component) -> list:
    if isinstance(address_or_component, str):
        address = address_or_component
    else:
        address = address_or_component.address

    return usystem.methods(address)


def annotations(address_or_component, method: str):
    if isinstance(address_or_component, str):
        address = address_or_component
    else:
        address = address_or_component.address

    return usystem.annotations(address, method)


def debug(*args):
    buf = ujson.dumps(args)
    _debug(buf)
