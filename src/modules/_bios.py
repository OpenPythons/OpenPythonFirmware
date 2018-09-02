from component import components, Component
from handler import monitor
import micropython
import uos

from system import debug
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


rootfs_address = "f9c77205-4210-4017-b831-cac8fa4f491d"

devices = components()
eeprom = devices["eeprom"]
computer = devices["computer"]
gpu = devices["gpu"]
fs = Component(rootfs_address, "filesystem")


def _cls():
    monitor.widthPos = 1
    monitor.heightPos = 1
    gpu.fill(1, 1, monitor.widthSize, monitor.heightSize, " ")


def mem_info():
    import gc
    gc.collect()
    micropython.mem_info()


cls = Command(_cls)
mem = Command(mem_info)


def main():
    uos.mount(FileSystem(rootfs_address), '/')

    with open("_bios.py", "w") as fp:
        fp.write("print(32)")

    print(eeprom)


if __name__ == '__main__':
    main()
