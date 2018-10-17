# machine
"functions related to the hardware"
from typing import Callable, Tuple


def debug(arg: str):
    pass


def hook_signal(func: Callable[[Tuple[str, Tuple]], None]):
    pass


def hook_stdin(func: Callable[[str], None]):
    pass


def hook_stdout(func: Callable[[str], None]):
    pass


def hook_stderr(func: Callable[[str], None]):
    pass
