#include "py/mpconfig.h"

#ifndef MPACK_CONFIG_H
#define MPACK_CONFIG_H 1

#define MPACK_READER 1
#define MPACK_EXPECT 1
#define MPACK_NODE 1
#define MPACK_WRITER 1
#define MPACK_COMPATIBILITY 0

#define MPACK_STDLIB 0
#define MPACK_STDIO 0

#define MPACK_STRINGS 1

#if MICROPY_ENABLE_GC

#include "py/gc.h"

#define MPACK_MALLOC(b) gc_alloc(b, false)
#define MPACK_REALLOC(ptr, n) gc_realloc(ptr, n, true)
#define MPACK_FREE(ptr) gc_free(ptr)
#endif

#endif
