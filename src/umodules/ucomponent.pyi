# ucomponent
"functions related to the components"
from typing import Dict, List, Any, Union, overload, Tuple, Optional


@overload
def get_list() -> Dict[str, str]:
    pass


@overload
def get_list(device: str) -> Dict[str, str]:
    pass


def get_type(address: str) -> Optional[str]:
    pass


def get_slot(address: str) -> Optional[int]:
    pass


def get_methods(address: str) -> List[str]:
    pass


def get_doc(address: str, method: str) -> Optional[str]:
    pass


def invoke(address: str, method: str, *args: Any) -> Union[Any, Tuple[Any]]:
    pass
