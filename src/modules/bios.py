def bios():
    globals().pop('bios')

    from machine import invoke, components, crash

    eeproms = components("eeprom")
    if not eeproms:
        crash("no bios found; install a configured EEPROM with Python")

    eeprom = eeproms[0]
    if True:
        invoke(eeprom, 'setLabel', "EEPROM (micropython)")
        invoke(eeprom, 'set', b"""python = []
from machine import invoke, components, crash
from uio import FileIO


init = '/init.py'


filesystems = components("filesystem")
def check_bootable(address):
    return address and address in filesystems and invoke(address, 'exists', init)


def load(address):
    file = invoke(address, 'open', init, 'r')
    
    try:
        buffer = []
        while True:
            buf = invoke(address, 'read', file, 4096)
            if not buf: break
            buffer.append(buf)
    finally:
        invoke(address, 'close', file)
        
    content = b"".join(buffer)
    return content.decode()


def main():
    address = invoke(__path__, 'getData').decode()
    if not check_bootable(address):
        invoke(__path__, 'setData', b'')
        for address in filesystems:
            if check_bootable(address):
                break
        else:
            crash("no bootable medium found")

    content = load(address)
    context = {'__name__': '__main__', '__path__': address}
    func = compile(content, init, "exec")
    exec(func, context)


if __name__ == '__main__':
    main()
""")

    data = invoke(eeprom, 'get')
    context = {'__name__': '__main__', '__path__': eeprom}

    func = compile(data, "<EEPROM>", "exec")
    exec(func, context)


bios()
