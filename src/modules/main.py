import ujson
import usystem

def syscall(*args):
    buf = ujson.dumps(args)
    usystem.example("?")
    return usystem.syscall(buf)

print(syscall())
