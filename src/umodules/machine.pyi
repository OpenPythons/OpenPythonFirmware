# machine
"functions related to the hardware"
from typing import Dict, List, Any, Union


# TODO: List to Tuple?


def invoke(address: str, method: str, *args: Any) -> Union[Any, List]:
    pass


def signal() -> List:
    pass


def components() -> Dict[str, str]:
    pass


def methods(address: str) -> List[str]:
    pass


def annotations(address: str, method: str) -> str:
    pass


def set_stdin_char(char: int) -> bool:
    pass


def get_stdout_str() -> int:
    pass


def print_hook(func):
    pass


def debug(*args):
    pass
