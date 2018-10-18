def bios():
    globals().pop('bios')

    from ucomputer import crash
    from ucomponent import invoke, get_list

    eeproms = get_list("eeprom")
    if not eeproms:
        crash("no bios found; install a configured EEPROM with Python")

    eeprom = eeproms[0]
    data = invoke(eeprom, 'get')

    comment = b"--[==[", b"]==]"
    if data.startswith(comment[0]) and comment[1] in data:
        data = data[len(comment[0]):].partition(comment[1])[0]

    context = {'__name__': '__main__', '__path__': eeprom}

    func = compile(data, "<EEPROM>", "exec")
    exec(func, context)


bios()
