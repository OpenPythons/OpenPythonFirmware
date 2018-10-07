import builtins
import sys

import machine
import micropython
import uos
from machine import components

from component import devices, Component
from handler import monitor
from vfs import FileSystem


class Command:
    def __init__(self, func):
        self.func = func

    def __call__(self, *args, **kwargs):
        return self.func(*args, **kwargs)

    def __repr__(self):
        result = self.func()
        if result is None:
            monitor.heightPos -= 1
            return ""
        else:
            return result


def _listdir(path="/"):
    print([path for path, *_ in uos.ilistdir(path)])


def _cls():
    monitor.widthPos = 1
    monitor.heightPos = 1
    gpu.fill(1, 1, monitor.widthSize, monitor.heightSize, " ")


def mem_info():
    import gc
    gc.collect()
    micropython.mem_info()


def _show(name=None):
    if name is None:
        help("modules")
        return

    module = __import__(name)
    help(module)


listdir = Command(_listdir)
cls = Command(_cls)
mem = Command(mem_info)
help = Command(help)
show = Command(_show)
reboot = Command(machine.reboot)
del Command, _listdir, _cls, mem_info, _show


def input(prompt=None):
    if prompt is not None:
        print(prompt, end="")

    read = sys.stdin.read
    write = sys.stdout.write
    buf = []
    ignore = 0
    while True:
        ch = read(1)
        if ignore > 0:
            ignore -= 1
            continue

        if ch == '\n':
            write(ch)
            break
        elif ch == '\b':
            buf.pop()
            write(ch + "---")  # TODO: why?
            continue
        else:
            write(ch)
            buf.append(ch)

    return ''.join(buf)


builtins.input = input


def main():
    global fs, eeprom, gpu

    rootfs_address = "f9c77205-4210-4017-b831-cac8fa4f491d"
    devs = devices()
    for key, value in devs.items():
        globals()[key] = value

    eeprom = devs["eeprom"]
    gpu = devs["gpu"]
    fs = Component(rootfs_address, "filesystem")
    extfs = None

    for key, value in components().items():
        if value == "filesystem":
            component = Component(key, value)
            if "INTERNAL" in component.list("/"):
                extfs = component
                break

    uos.mount(FileSystem(rootfs_address), '/')
    if extfs is not None:
        uos.mount(FileSystem(extfs.address), '/opos', readonly=True)

    eeprom.setLabel("EEPROM (micropython)")
    eeprom.setData(b"")
    eeprom.set(b"""import gc
gc.collect()
gc.threshold(gc.mem_free() // 4 + gc.mem_alloc())        
from opos.lib.shell import spawn
spawn("/opos/bin/shell.py")
""")

    data = eeprom.get()

    try:
        exec(data)
    except BaseException as e:
        print(type(e), e)


if __name__ == '__main__':
    main()
