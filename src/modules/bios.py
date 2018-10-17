def bios():
    globals().pop('bios')

    from ucomputer import crash
    from ucomponent import invoke, components

    eeproms = components("eeprom")
    if not eeproms:
        crash("no bios found; install a configured EEPROM with Python")

    eeprom = eeproms[0]
    data = invoke(eeprom, 'get')
    context = {'__name__': '__main__', '__path__': eeprom}

    func = compile(data, "<EEPROM>", "exec")
    exec(func, context)


bios()
