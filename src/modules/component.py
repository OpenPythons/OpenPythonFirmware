from system import syscall, components


class Component:
    def __init__(self, address, type):
        self.address = address
        self.type = type

    def __getattr__(self, name):
        return lambda *args: syscall(self.address, name, *args)

    def __dir__(self):
        return dir(object) + ["address", "type"]

    def __repr__(self):
        return "Component<{0}:{1}>".format(self.type, self.address)

    @classmethod
    def list(self):
        return {value: Component(key, value) for key, value in components().items()}



class Monitor:
    def __init__(self, gpu):
        self.gpu = gpu
        self.widthSize = 50
        self.heightSize = 16
        self.widthPos = 1
        self.heightPos = 1
        self.backCount = 0

    def scroll(self):
        self.gpu.copy(1, 2, self.widthSize, self.heightSize, 0, - 1)
        self.gpu.fill(1, self.heightSize, self.widthSize, 1, " ")

    def putChar(self, char):
        if char == "\r":
            self.widthPos = 1
        if char == "\n":
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
        elif char == "\b": # escape required...
            self.widthPos -= 1
            self.gpu.set(self.widthPos, self.heightPos, " ")
            self.backCount = 3
        else:
            self.gpu.set(self.widthPos, self.heightPos, char)
            self.widthPos += 1
