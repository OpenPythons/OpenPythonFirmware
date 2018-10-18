def bios():
    globals().pop('bios')

    from ucomputer import crash
    from ucomponent import invoke, get_list

    eeproms = get_list("eeprom")
    if not eeproms:
        crash("no bios found; install a configured EEPROM with Python")

    eeprom = eeproms[0]
    if True:
        invoke(eeprom, 'setLabel', "EEPROM (micropython)")
        invoke(eeprom, 'set', b"""#!micropython


def main():
    globals().pop('main')
    from ucomponent import invoke, get_list as get_components
    from ucomputer import crash, get_computer_address

    def component(t):
        seq = get_components(t)
        return seq[0] if seq else None

    def check_bootable(filesystems, address):
        return address in filesystems and invoke(address, 'exists', '/init.py')

    eeprom = __path__
    filesystems = get_components("filesystem")

    address = invoke(eeprom, 'getData').decode()
    if not check_bootable(filesystems, address):
        invoke(__path__, 'setData', b'')
        for address in filesystems:
            if check_bootable(filesystems, address):
                invoke(eeprom, 'setData', address.encode())
                break
        else:
            crash("no bootable medium found")

    computer = get_computer_address()
    invoke(computer, 'beep', 1000, 0.2)

    gpu = component("gpu")
    monitor = component("monitor")
    if gpu and monitor:
        invoke(gpu, "bind", monitor)

    def load(address):
        handle = invoke(address, 'open', '/init.py', 'r')
        buffer = []

        try:
            while True:
                buf = invoke(address, 'read', handle, 4096)
                if not buf: break
                buffer.append(buf)
        finally:
            invoke(address, 'close', handle)
            handle.dispose()

        content = b"".join(buffer)
        return content.decode()

    content = load(address)
    context = {'__name__': '__main__', '__path__': address}
    func = compile(content, '/init.py', "exec")
    exec(func, context)


if __name__ == '__main__':
    main()
""")

    comment = b"--[==[", b"]==]"

    data = invoke(eeprom, 'get')
    if data.startswith(comment[0]) and comment[1] in data:
        data = data[len(comment[0]):].partition(comment[1])[0]

    context = {'__name__': '__main__', '__path__': eeprom}

    func = compile(data, "<EEPROM>", "exec")
    exec(func, context)


bios()
