#ifndef OPENPIE_MACHINE_H
#define OPENPIE_MACHINE_H

int main(int argc, char **argv);
void _exit(int status);

void nlr_jump_fail(void *val);
void NORETURN __fatal_error(const char *msg);

#endif // OPENPIE_MACHINE_H
