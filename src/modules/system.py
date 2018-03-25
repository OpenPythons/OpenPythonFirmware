import ujson
import usystem

def syscall(*req):
    req_buf = ujson.dumps(req)
    res_buf = usystem.syscall(req_buf)
    return ujson.loads(res_buf)
