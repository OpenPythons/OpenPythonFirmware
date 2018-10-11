from typing import Callable, Any


def repl_input() -> str:
    pass


def repl_compile(code: str, globals: dict) -> Callable[[], Any]:
    pass


def repl_call(func: Callable[[], Any], locals: dict) -> Any:
    pass
