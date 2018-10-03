# ure
"simple regular expressions"

DEBUG: int


class _RegEx:
    def match(self, string: str) -> _Match:
        "Similar to the module-level functions match() and search(). Using methods is (much) more efficient if the same regex is applied to multiple strings."
        pass

    def search(self, string: str) -> _Match:
        "Similar to the module-level functions match() and search(). Using methods is (much) more efficient if the same regex is applied to multiple strings."
        pass


class _Match:
    def group(self, index: int = 0) -> str:
        "Return matching (sub)string. index is 0 for entire match, 1 and above for each capturing group. Only numeric groups are supported."
        pass


def compile(regex_str: str) -> _RegEx:
    "Compile regular expression, return regex object."
    pass


def match(regex_str: str, string: str) -> _Match:
    "Compile regex_str and match against string. Match always happens from starting position in a string."
    pass


def search(regex_str, string) -> _Match:
    "Compile regex_str and search it in a string. Unlike match, this will search string for first position which matches regex (which still may be 0 if regex is anchored)."
    pass
