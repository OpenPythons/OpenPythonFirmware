import ujson
import usystem
from usystem import debug as _debug
signals = []

def parse(buf):
    return ujson.loads(buf) if buf is not None else buf


def syscall(*req):
    req_buf = ujson.dumps(req)
    res_buf = usystem.syscall(req_buf)
    return parse(res_buf)


def signal():
    return signals.pop(0) if signals else None


def components():
    return parse(usystem.components())


def debug(*args):
    buf = ujson.dumps(args)
    _debug(buf)
