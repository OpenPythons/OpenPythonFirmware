from typing import Callable, Any

MICROPY_GIT_TAG: str
MICROPY_BUILD_DATE: str
MICROPY_HW_BOARD_NAME: str
MICROPY_HW_MCU_NAME: str


def repl_input() -> str:
    pass


def repl_compile(code: str, globals: dict) -> Callable[[], Any]:
    pass


def repl_call(func: Callable[[], Any], locals: dict) -> Any:
    pass
