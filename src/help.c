#include "py/builtin.h"

const char *unicorn_help_text =
"Welcome to MicroPython running under Unicorn!\n"
"\n"
"Control commands:\n"
"  CTRL-A        -- on a blank line, enter raw REPL mode\n"
"  CTRL-B        -- on a blank line, enter normal REPL mode\n"
"  CTRL-C        -- interrupt a running program\n"
"  CTRL-D        -- on a blank line, do a soft reset of the board\n"
"  CTRL-E        -- on a blank line, enter paste mode\n"
"\n"
"For further help on a specific object, type help(obj)\n"
#if MICROPY_PY_BUILTINS_HELP_MODULES
"For a list of available modules, type help('modules')\n"
#endif
;
