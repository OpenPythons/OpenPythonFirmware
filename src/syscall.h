#ifndef OPENPIE_SYSCALL_H
#define OPENPIE_SYSCALL_H

#include "syscall_arch.h"

typedef enum {
    SYS_UNKNOWN = 0,
    SYS_CONTROL = 1,
    SYS_INVOKE = 2,
    SYS_REQUEST = 3,
    SYS_VFS = 4,
    SYS_DEBUG = 5,
    SYS_LEGACY = 6,
} syscall_number;

typedef enum {
    SYS_CONTROL_SHUTDOWN = 1,
    SYS_CONTROL_REBOOT = 2,
    SYS_CONTROL_CRASH = 3,
    SYS_CONTROL_RETURN = 4,
} syscall_control_number;

typedef enum {
    // VERSION?
    SYS_REQUEST_COMPONENTS = 1,
    SYS_REQUEST_METHODS = 2,
    SYS_REQUEST_ANNOTATIONS = 3,
} syscall_request_number;

// typedef enum {
// } syscall_vfs_number;

typedef enum {
    SYS_LEGACY_usystem_get_stdout_str = 1
} syscall_legacy_number;

#endif //OPENPIE_SYSCALL_H
