import ujson
import usystem
from usystem import debug as _debug

__all__ = ["syscall", "signal", "components", "methods", "debug", "ExternalException"]


def parse(buf):
    return ujson.loads(buf) if buf is not None else buf


class ExternalException(Exception):
    pass


def syscall(*req):
    req_buf = ujson.dumps(req)
    res_buf = usystem.syscall(req_buf)
    res = parse(res_buf)
    result, error = res
    if error is not None:
        raise ExternalException(error)

    if result is None or len(result) == 0:
        return None
    elif len(result) == 1:
        return result[0]
    else:
        return tuple(result)


def raw_syscall(*req):
    req_buf = ujson.dumps(req)
    res_buf = usystem.syscall(req_buf)
    res = parse(res_buf)
    result, error = res
    if error is not None:
        raise ExternalException(error)

    return tuple(result)


def signal():
    res_buf = usystem.signal()
    return parse(res_buf)


def components():
    return parse(usystem.components())


def methods(address_or_component) -> list:
    if isinstance(address_or_component, str):
        address = address_or_component
    else:
        address = address_or_component.address

    return parse(usystem.methods(address))


def annotations(address_or_component, method: str):
    if isinstance(address_or_component, str):
        address = address_or_component
    else:
        address = address_or_component.address

    req_buf = ujson.dumps((address, method))
    return parse(usystem.annotations(req_buf))


def debug(*args):
    buf = ujson.dumps(args)
    _debug(buf)
