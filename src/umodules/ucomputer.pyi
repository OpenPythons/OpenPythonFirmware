# machine
"functions related to the computer"
from typing import Dict, List, Any, Union, overload, Tuple


def shutdown():
    pass


def reboot():
    pass


def crash(message: str):
    pass


def push_signal(name: str, *args: Any):
    pass


def pop_signal(ticks: int) -> Tuple[str, Tuple[Any]]:
    pass


def get_cost_per_tick() -> float:
    pass


def last_error() -> str:
    pass


def users() -> List[str]:
    pass


def add_user(user: str):
    pass


def remove_user(user: str) -> bool:
    pass


def get_computer_address() -> str:
    pass


def get_tmp_address() -> str:
    pass


@overload
def beep(a: float, b: float):
    pass


@overload
def beep(s: str):
    pass
