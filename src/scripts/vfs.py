from component import Component
from system import components
from uio import FileIO

class FileSystem:
    def __init__(self, address):
        self.fs = Component(address, components()[address])
        self.address = address

    def mount(self, readonly, mkfs):
        print(self.address, 'mount', readonly, mkfs)

    def umount(self):
        print(self.address, 'umount')

    def ilistdir(self, dir):
        print(self.address, 'ilistdir', dir)
        return iter([('a%d' % self.address, 0, 0)])

    def chdir(self, dir):
        print(self.address, 'chdir', dir)

    def getcwd(self):
        print(self.address, 'getcwd')
        return 'dir%d' % self.address

    def mkdir(self, path):
        print(self.address, 'mkdir', path)

    def remove(self, path):
        print(self.address, 'remove', path)

    def rename(self, old_path, new_path):
        print(self.address, 'rename', old_path, new_path)

    def rmdir(self, path):
        print(self.address, 'rmdir', path)

    def stat(self, path):
        print(self.address, 'stat', path)
        return (self.address,)

    def statvfs(self, path):
        print(self.address, 'statvfs', path)
        return (self.address,)

    def open(self, file, mode):
        # syscall(fs.address, "read", file.handle, ...)
        # vfs_open(fs.address, file. mode) -> file.handle
        return FileIO(self.fs.address, file, mode)

    def __repr__(self):
        return "<FileSystem: {!r}>".format(self.address)
