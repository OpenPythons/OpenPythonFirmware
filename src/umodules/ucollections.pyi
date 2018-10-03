# ucollections
"collection and container types"
from typing import NamedTuple, Deque


def deque() -> Deque:
    pass


def namedtuple(name: str, fields: tuple) -> NamedTuple:
    "This is factory function to create a new namedtuple type with a specific name and set of fields. A namedtuple is a subclass of tuple which allows to access its fields not just by numeric index, but also with an attribute access syntax using symbolic field names. Fields is a sequence of strings specifying field names. For compatibility with CPython it can also be a a string with space-separated field named (but this is less efficient)."
    pass


class OrderedDict(dict):
    "dict type subclass which remembers and preserves the order of keys added. When ordered dict is iterated over, keys/items are returned in the order they were added"
    pass
