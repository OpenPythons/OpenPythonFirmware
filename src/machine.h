#ifndef OPENPYTHON_MACHINE_H
#define OPENPYTHON_MACHINE_H

#include "py/objexcept.h"

int main(int argc, char **argv);
void _exit(int status);

void nlr_jump_fail(void *val);
void NORETURN __fatal_error(const char *msg);

#endif // OPENPYTHON_MACHINE_H
