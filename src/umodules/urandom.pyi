from typing import Union, Sequence, TypeVar

T = TypeVar('T')


def getrandbits(n: int) -> int:
    pass


def seed(a: int) -> None:
    pass


def randrange(start: int, stop: Union[None, int] = None, step: int = None) -> int:
    pass


def randint(start: int, end: int) -> int:
    pass


def choice(seq: Sequence[T]) -> T:
    pass


def random() -> float:
    pass


def uniform(a: float, b: float) -> float:
    pass
