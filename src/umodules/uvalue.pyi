# uvalue
from typing import Callable, Any, List, Union, Optional


class UValue:
    value: Any

    def __del__(self):
        pass

    def dispose(self):
        pass


def hook_value(func: Callable[[object], object]):
    pass


def methods(address: str) -> List[str]:
    pass


def doc(address: str, method: Optional[str] = None) -> str:
    pass


def call(value: UValue, *args: Any):
    pass


def apply(value: UValue, *args: Any):
    pass


def unapply(value: UValue, *args: Any):
    pass


def invoke(value: UValue, method: str, *args: Any) -> Union[Any, Tuple[Any]]:
    pass
