import ujson
import usystem
from usystem import debug as _debug


def parse(buf):
    return ujson.loads(buf) if buf is not None else buf


def syscall(*req):
    req_buf = ujson.dumps(req)
    res_buf = usystem.syscall(req_buf)
    return parse(res_buf)


def signal():
    res_buf = usystem.signal()
    return parse(res_buf)


def components():
    return parse(usystem.components())


def debug(*args):
    buf = ujson.dumps(args)
    _debug(buf)
