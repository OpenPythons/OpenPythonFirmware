#include "py/builtin.h"

const char umport_help_text[] =
"Welcome to MicroPython running under umport!\n"
"\n"
"For further help on a specific object, type help(obj)\n"
#if MICROPY_PY_BUILTINS_HELP_MODULES
"For a list of available modules, type help('modules')\n"
#endif
;
