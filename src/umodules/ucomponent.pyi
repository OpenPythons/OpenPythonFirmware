# ucomponent
"functions related to the components"
from typing import Dict, List, Any, Union, overload, Tuple


@overload
def components() -> Dict[str, str]:
    pass


@overload
def components(device: str) -> List[str]:
    pass


def component_count() -> int:
    pass


def max_components() -> int:
    pass


def methods(address: str) -> List[str]:
    pass


def doc(address: str, method: str) -> str:
    pass


def invoke(address: str, method: str, *args: Any) -> Union[Any, Tuple[Any]]:
    pass
