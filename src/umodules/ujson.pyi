# ujson
"JSON encoding and decoding"
from typing import Any, IO


def dumps(obj: Any) -> str:
    "Return obj represented as a JSON string."
    pass


def loads(str: str) -> Any:
    "Parse the JSON str and return an object. Raises ValueError if the string is not correctly formed."
    pass


def load(stream: IO) -> Any:
    pass


def dump(stream: IO, obj: Any):
    pass
