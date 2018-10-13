#ifndef OPENPIE_SYSCALL_H
#define OPENPIE_SYSCALL_H

#include "syscall_arch.h"
#include "syscall_table.h"

typedef struct _syscall_result_t {
    int status;
    byte *buf;
    size_t len;
} syscall_result_t;

#endif //OPENPIE_SYSCALL_H
