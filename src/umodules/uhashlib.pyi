# uhashlib
"hashing algorithms"


# noinspection PyPep8Naming
class sha256:
    "Create an SHA256 hasher object and optionally feed data into it."

    def __init__(self, data: bytes = None):
        pass

    def update(self, data: bytes):
        "Feed more binary data into hash."
        pass

    def digest(self) -> bytes:
        "Return hash for all data passed through hash, as a bytes object. After this method is called, more data cannot be fed into the hash any longer."
        pass
