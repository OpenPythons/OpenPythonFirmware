# machine
"functions related to the hardware"
from typing import Dict, List, Any, Union, Callable, overload


# TODO: List to Tuple?

def repl_input() -> str:
    pass


def repl_compile(code: str, globals: dict) -> Callable[[], Any]:
    pass


def repl_call(func: Callable[[], Any], locals: dict) -> Any:
    pass


def invoke(address: str, method: str, *args: Any) -> Union[Any, List]:
    pass


def signal(ticks: int) -> List:
    pass


@overload
def components() -> Dict[str, str]:
    pass


@overload
def components(device: str) -> List[str]:
    pass


def methods(address: str) -> List[str]:
    pass


def annotations(address: str, method: str) -> str:
    pass


def set_stdin_char(char: int) -> bool:
    pass


def get_stdout_str() -> int:
    pass


def debug(*args):
    pass


def shutdown():
    pass


def reboot():
    pass


def hook_input(func):
    pass


def hook_print(func):
    pass


def hook_signal(func):
    pass


def crash(message: str):
    return None
