from machine import invoke, methods, annotations
import machine


def devices():
    return {value: Component(key, value) for key, value in machine.components().items()}


class ComponentMethod:
    def __init__(self, component, name):
        self.component = component
        self.name = name

    def __call__(self, *args):
        return invoke(self.component.address, self.name, *args)

    @property
    def doc(self):
        return annotations(self.component.address, self.name)

    def __repr__(self):
        doc = self.doc
        if doc:
            doc = "\n" + doc.replace(" -- ", "\n")
        else:
            doc = ""

        return "ComponentMethod<{0!r}, {1!r}>{2}".format(self.component, self.name, doc)


class Component:
    def __init__(self, address, type):
        self.address = address
        self.type = type

    def __getattr__(self, name):
        return ComponentMethod(self, name)

    def __dir__(self):
        return dir(object()) + ["address", "type"] + methods(self.address)

    def __repr__(self):
        return "Component<{0}:{1}>".format(self.type, self.address)


class Monitor:
    def __init__(self, gpu):
        self.gpu = gpu
        self.widthSize = 80
        self.heightSize = 25
        self.widthPos = 1
        self.heightPos = 1
        self.backCount = 0
        self.blinkState = False

    def __getattr__(self, item):
        return getattr(self.gpu, item)

    def clearBlink(self):
        if self.blinkState:
            self.blink()

    def setBlink(self):
        if not self.blinkState:
            self.blink()

    def blink(self):
        self.blinkState = not self.blinkState
        self.gpu.set(self.widthPos, self.heightPos, "_" if self.blinkState else " ")

    def scroll(self):
        self.gpu.copy(1, 2, self.widthSize, self.heightSize, 0, - 1)
        self.gpu.fill(1, self.heightSize, self.widthSize, 1, " ")

    def put(self, buf: str):
        for char in buf:
            self.putChar(char)

    def putChar(self, char):
        if self.blinkState:
            self.blink()

        if char == "\n":
            self.widthPos = 1
            self.heightPos += 1

        if self.widthPos > self.widthSize:
            self.widthPos = 1
            self.heightPos += 1

        if self.heightPos > self.heightSize:
            self.scroll()
            self.heightPos = self.heightSize

        if char == "\r" or char == "\n":
            return

        if self.backCount > 0:
            self.backCount -= 1
        elif char == "\b":  # escape required...
            self.widthPos -= 1
            self.gpu.set(self.widthPos, self.heightPos, " ")
            self.backCount = 3
        else:
            self.gpu.set(self.widthPos, self.heightPos, char)
            self.widthPos += 1
