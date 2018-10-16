def bios():
    globals().pop('bios')

    from ucomputer import crash
    from ucomponents import invoke, components

    eeproms = components("eeprom")
    if not eeproms:
        crash("no bios found; install a configured EEPROM with Python")

    eeprom = eeproms[0]
    if False:
        invoke(eeprom, 'setLabel', "EEPROM (micropython)")
        invoke(eeprom, 'set', b"""#!micropython
from ucomputer import crash, get_computer_address
from ucomponents import invoke, components
from uio import FileIO


init = '/init.py'


def get_component(t):
    seq = components(t)
    return seq[0] if seq else None


filesystems = components("filesystem")
def check_bootable(address):
    return address and address in filesystems and invoke(address, 'exists', init)


def load(address):
    handle = invoke(address, 'open', init, 'r')
    
    try:
        buffer = []
        while True:
            buf = invoke(address, 'read', handle, 4096)
            if not buf: break
            buffer.append(buf)
    finally:
        invoke(address, 'close', handle)
        handle.dispose()
        
    content = b"".join(buffer)
    return content.decode()

def main():
    address = invoke(__path__, 'getData').decode()
    if not check_bootable(address):
        invoke(__path__, 'setData', b'')
        for address in filesystems:
            if check_bootable(address):    
                invoke(__path__, 'setData', address.encode())
                break
        else:
            crash("no bootable medium found")

    computer = get_computer_address()
    invoke(computer, 'beep', 1000, 0.2)

    gpu = get_component("gpu")
    monitor = get_component("monitor")
    if gpu and monitor:
        invoke(gpu, "bind", monitor)

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
