import micropython
import uos

from component import devices, Component
from handler import monitor
from machine import components
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

print("extfs:", extfs)


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


listdir = Command(_listdir)
cls = Command(_cls)
mem = Command(mem_info)
help = Command(help)


def _show(name=None):
    if name is None:
        help("modules")
        return

    module = __import__(name)
    help(module)


show = Command(_show)

# noinspection PyUnresolvedReferences
from umsgpack import dumps, loads


def main():
    uos.mount(FileSystem(rootfs_address), '/')
    if extfs is not None:
        uos.mount(FileSystem(extfs.address), '/opos', readonly=True)

    import opos.example

    with open("_bios.py", "w") as fp:
        fp.write("print(32)")

    print(eeprom)


if __name__ == '__main__':
    main()
