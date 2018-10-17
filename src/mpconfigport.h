#include <stdint.h>

// Object representation
#define MICROPY_OBJ_REPR (MICROPY_OBJ_REPR_A)

// Memory allocation policy
#define MICROPY_GC_ALLOC_THRESHOLD (1)
#define MICROPY_ALLOC_PATH_MAX (255)
#define MICROPY_MODULE_DICT_SIZE (1)
#define MICROPY_MALLOC_USES_ALLOCATED_SIZE (1)
#define MICROPY_QSTR_BYTES_IN_LEN (1)
#define MICROPY_QSTR_BYTES_IN_HASH (2)
#define MICROPY_STACKLESS (1)
#define MICROPY_STACKLESS_STRICT (0)

// MicroPython emitters
#define MICROPY_PERSISTENT_CODE_LOAD (1)
#define MICROPY_PERSISTENT_CODE_SAVE (0)
#define MICROPY_EMIT_THUMB (0)
#define MICROPY_EMIT_INLINE_THUMB (0)
#define MICROPY_EMIT_INLINE_THUMB_ARMV7M (0)
#define MICROPY_EMIT_INLINE_THUMB_FLOAT (0)
#define MICROPY_EMIT_ARM (0)

// Compiler configuration
#define MICROPY_ENABLE_COMPILER (1)
#define MICROPY_DYNAMIC_COMPILER (0)
#define MICROPY_COMP_CONST_FOLDING (1)
#define MICROPY_COMP_MODULE_CONST (1)
#define MICROPY_COMP_CONST (1)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN (1)
#define MICROPY_COMP_TRIPLE_TUPLE_ASSIGN (1)
#define MICROPY_COMP_RETURN_IF_EXPR (1)

// Internal debugging stuff
#define MICROPY_MEM_STATS (1)
#define MICROPY_DEBUG_PRINTERS (0)
#define MICROPY_DEBUG_VERBOSE (0)

// Optimisations
#define MICROPY_OPT_COMPUTED_GOTO (1)
#define MICROPY_OPT_CACHE_MAP_LOOKUP_IN_BYTECODE (1)
#define MICROPY_OPT_MPZ_BITWISE (1)

// Python internal features
#define MICROPY_ENABLE_EXTERNAL_IMPORT (1)
#define MICROPY_READER_POSIX (0)
#define MICROPY_READER_VFS (1)
#define MICROPY_VM_HOOK_INIT
#define MICROPY_VM_HOOK_LOOP
#define MICROPY_VM_HOOK_RETURN
#define MICROPY_ENABLE_GC (1)
#define MICROPY_ENABLE_FINALISER (1)
#define MICROPY_ENABLE_PYSTACK (1)
#define MICROPY_PYSTACK_ALIGN (8)
#define MICROPY_STACK_CHECK (1)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF (1)
#define MICROPY_EMERGENCY_EXCEPTION_BUF_SIZE (256)
#define MICROPY_KBD_EXCEPTION (1)
#define MICROPY_ASYNC_KBD_INTR (1)
#define MICROPY_HELPER_REPL (1)
#define MICROPY_REPL_AUTO_INDENT (0)
#define MICROPY_REPL_EVENT_DRIVEN (0)
#define MICROPY_HELPER_LEXER_UNIX (1)
#define MICROPY_LONGINT_IMPL (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_ENABLE_SOURCE_LINE (1)
#define MICROPY_ENABLE_DOC_STRING (1)
#define MICROPY_ERROR_REPORTING (MICROPY_ERROR_REPORTING_DETAILED)
#define MICROPY_WARNINGS (1)
#define MICROPY_FLOAT_IMPL (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_PY_BUILTINS_COMPLEX (1)
#define MICROPY_FLOAT_HIGH_QUALITY_HASH (1)
#define MICROPY_CPYTHON_COMPAT (1)
#define MICROPY_FULL_CHECKS (1)
#define MICROPY_STREAMS_NON_BLOCK (0)
#define MICROPY_STREAMS_POSIX_API (0)
#define MICROPY_MODULE_BUILTIN_INIT (1)
#define MICROPY_MODULE_WEAK_LINKS (1)
#define MICROPY_MODULE_FROZEN_STR (1)
#define MICROPY_MODULE_FROZEN_MPY (1)
#define MICROPY_CAN_OVERRIDE_BUILTINS (1)
#define MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG (1)
#define MICROPY_USE_INTERNAL_ERRNO (1)
#define MICROPY_USE_INTERNAL_PRINTF (1)
#define MICROPY_ENABLE_SCHEDULER (1)
#define MICROPY_SCHEDULER_DEPTH (4)
#define MICROPY_VFS (1)

// Fine control over Python builtins, classes, modules, etc
#define MICROPY_MULTIPLE_INHERITANCE (1)
#define MICROPY_PY_FUNCTION_ATTRS (1)
#define MICROPY_PY_DESCRIPTORS (1)
#define MICROPY_PY_DELATTR_SETATTR (1)
#define MICROPY_PY_ASYNC_AWAIT (1)
#define MICROPY_PY_GENERATOR_PEND_THROW (1)
#define MICROPY_PY_STR_BYTES_CMP_WARN (1)
#define MICROPY_PY_BUILTINS_STR_UNICODE (1)
#define MICROPY_PY_BUILTINS_STR_CENTER (1)
#define MICROPY_PY_BUILTINS_STR_PARTITION (1)
#define MICROPY_PY_BUILTINS_STR_SPLITLINES (1)
#define MICROPY_PY_BUILTINS_BYTEARRAY (1)
#define MICROPY_PY_BUILTINS_MEMORYVIEW (1)
#define MICROPY_PY_BUILTINS_SET (1)
#define MICROPY_PY_BUILTINS_SLICE (1)
#define MICROPY_PY_BUILTINS_SLICE_ATTRS (1)
#define MICROPY_PY_BUILTINS_FROZENSET (1)
#define MICROPY_PY_BUILTINS_PROPERTY (1)
#define MICROPY_PY_BUILTINS_RANGE_ATTRS (1)
#define MICROPY_PY_BUILTINS_RANGE_BINOP (1)
#define MICROPY_PY_BUILTINS_TIMEOUTERROR (1)
#define MICROPY_PY_ALL_SPECIAL_METHODS (1)
#define MICROPY_PY_ALL_INPLACE_SPECIAL_METHODS (1)
#define MICROPY_PY_REVERSE_SPECIAL_METHODS (1)
#define MICROPY_PY_BUILTINS_COMPILE (1)
#define MICROPY_PY_BUILTINS_ENUMERATE (1)
#define MICROPY_PY_BUILTINS_EVAL_EXEC (1)
#define MICROPY_PY_BUILTINS_EXECFILE (1)
#define MICROPY_PY_BUILTINS_FILTER (1)
#define MICROPY_PY_BUILTINS_REVERSED (1)
#define MICROPY_PY_BUILTINS_NOTIMPLEMENTED (1)
#define MICROPY_PY_BUILTINS_MIN_MAX (1)
#define MICROPY_PY_BUILTINS_POW3 (1)
#define MICROPY_PY_BUILTINS_HELP (1)
#define MICROPY_PY_BUILTINS_HELP_TEXT openpie_help_text
#define MICROPY_PY_BUILTINS_HELP_MODULES (1)
#define MICROPY_PY___FILE__ (1)
#define MICROPY_PY_MICROPYTHON_MEM_INFO (1)
#define MICROPY_PY_MICROPYTHON_STACK_USE (1)
#define MICROPY_PY_ARRAY (1)
#define MICROPY_PY_ARRAY_SLICE_ASSIGN (1)
#define MICROPY_PY_ATTRTUPLE (1)
#define MICROPY_PY_COLLECTIONS (1)
#define MICROPY_PY_COLLECTIONS_DEQUE (1)
#define MICROPY_PY_COLLECTIONS_ORDEREDDICT (1)
#define MICROPY_PY_COLLECTIONS_NAMEDTUPLE__ASDICT (1)
#define MICROPY_PY_MATH (1)
#define MICROPY_PY_MATH_SPECIAL_FUNCTIONS (1)
#define MICROPY_PY_CMATH (1)
#define MICROPY_PY_GC (1)
#define MICROPY_PY_GC_COLLECT_RETVAL (1)
#define MICROPY_PY_IO (1)
#define MICROPY_PY_IO_RESOURCE_STREAM (1)
#define MICROPY_PY_IO_FILEIO (1)
#define MICROPY_PY_IO_BYTESIO (1)
// MICROPY_PY_IO_BUFFEREDWRITER make error with `unused-variable`
#define MICROPY_PY_IO_BUFFEREDWRITER (0)
#define MICROPY_PY_STRUCT (1)
#define MICROPY_PY_SYS (1)
#define MICROPY_PY_SYS_MAXSIZE (1)
#define MICROPY_PY_SYS_MODULES (1)
#define MICROPY_PY_SYS_EXC_INFO (1)
#define MICROPY_PY_SYS_EXIT (1)
#define MICROPY_PY_SYS_GETSIZEOF (1)
#define MICROPY_PY_SYS_STDFILES (1)
#define MICROPY_PY_SYS_STDIO_BUFFER (1)
#define MICROPY_PY_UERRNO (1)
#define MICROPY_PY_UERRNO_ERRORCODE (1)
#define MICROPY_PY_USELECT (0)
#define MICROPY_PY_UTIME_MP_HAL (1)
#define MICROPY_PY_UTIME_TICKS_PERIOD (MP_SMALL_INT_POSITIVE_MASK + 1)
#define MICROPY_PY_THREAD (0)
#define MICROPY_PY_THREAD_GIL (MICROPY_PY_THREAD)
#define MICROPY_PY_THREAD_GIL_VM_DIVISOR (32)

// Extended modules
#define MICROPY_PY_UCTYPES (1)
#define MICROPY_PY_UZLIB (1)
#define MICROPY_PY_UJSON (1)
#define MICROPY_PY_URE (1)
#define MICROPY_PY_URE_MATCH_GROUPS (1)
#define MICROPY_PY_URE_MATCH_SPAN_START_END (1)
#define MICROPY_PY_URE_SUB (1)
#define MICROPY_PY_UHEAPQ (1)
#define MICROPY_PY_UTIMEQ (1)
#define MICROPY_PY_UHASHLIB (1)
#define MICROPY_PY_UHASHLIB_MD5 (0)
#define MICROPY_PY_UHASHLIB_SHA1 (0)
#define MICROPY_PY_UHASHLIB_SHA256 (1)
#define MICROPY_PY_UCRYPTOLIB (0)
#define MICROPY_PY_UBINASCII (1)
#define MICROPY_PY_UBINASCII_CRC32 (1)
#define MICROPY_PY_URANDOM (1)
#define MICROPY_PY_URANDOM_EXTRA_FUNCS (1)
#define MICROPY_PY_MACHINE (1)
#define MICROPY_PY_MACHINE_PULSE (0)
#define MICROPY_PY_MACHINE_I2C (0)
#define MICROPY_PY_MACHINE_SPI (0)
#define MICROPY_PY_USSL (0)
#define MICROPY_PY_USSL_FINALISER (0)
#define MICROPY_PY_WEBSOCKET (0)
#define MICROPY_PY_FRAMEBUF (0)
#define MICROPY_PY_BTREE (0)

// Miscellaneous settings
extern const struct _mp_print_t debug_print;
#define MICROPY_DEBUG_PRINTER (&debug_print)
#define MICROPY_OBJ_BASE_ALIGNMENT
#define MICROPY_BEGIN_ATOMIC_SECTION() (0)
#define MICROPY_END_ATOMIC_SECTION(state) (void)(state)
#define STATIC static
#define MICROPY_MAKE_POINTER_CALLABLE(p) ((void*)((mp_uint_t)(p) | 1))
#define UINT_FMT "%u"
#define INT_FMT "%d"

// Machine settings
#define MICROPY_HW_BOARD_NAME "OpenPie"
#define MICROPY_HW_MCU_NAME "Cortex-M0"
#define MICROPY_OPENPIE_VFS (1)

#define MP_SSIZE_MAX (0x7fffffff)
#define MP_NEED_LOG2 (1)

typedef int mp_int_t; // must be pointer size
typedef unsigned mp_uint_t; // must be pointer size
typedef long mp_off_t;

#define MP_PLAT_PRINT_STRN(str, len) mp_hal_stdout_tx_strn_cooked(str, len)
#define MICROPY_QSTR_EXTRA_POOL mp_qstr_frozen_const_pool

#include <alloca.h>

// Exceptions
extern const struct _mp_obj_type_t mp_type_SystemError;

// Builtins
#define MP_STATE_PORT MP_STATE_VM

#define MICROPY_PORT_BUILTINS \
    { MP_OBJ_NEW_QSTR(MP_QSTR_open), (mp_obj_t)&mp_builtin_open_obj }, \
    { MP_ROM_QSTR(MP_QSTR_SystemError), MP_ROM_PTR(&mp_type_SystemError) }, \

#define MICROPY_PORT_CONSTANTS

#define MICROPY_PORT_ROOT_POINTERS \
    const char *readline_hist[8]; \
    mp_obj_t signal_hook_obj; \
    mp_obj_t stdin_hook_obj; \
    mp_obj_t stdout_hook_obj; \
    mp_obj_t stderr_hook_obj; \
    mp_obj_t object_hook_obj; \

#define mp_type_fileio mp_type_vfs_openpie_fileio
#define mp_type_textio mp_type_vfs_openpie_textio

#define mp_import_stat mp_vfs_import_stat
#define mp_builtin_open mp_vfs_open
#define mp_builtin_open_obj mp_vfs_open_obj

// Modules
extern const struct _mp_obj_module_t mp_module_machine;
extern const struct _mp_obj_module_t mp_module_ucode;
extern const struct _mp_obj_module_t mp_module_ucomponent;
extern const struct _mp_obj_module_t mp_module_ucomputer;
extern const struct _mp_obj_module_t mp_module_uos;
extern const struct _mp_obj_module_t mp_module_uimp;
extern const struct _mp_obj_module_t mp_module_utime;
extern const struct _mp_obj_module_t mp_module_umsgpack;
extern const struct _mp_obj_module_t mp_module_uvalue;

#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_ROM_QSTR(MP_QSTR_machine), MP_ROM_PTR(&mp_module_machine) }, \
    { MP_ROM_QSTR(MP_QSTR_ucode), MP_ROM_PTR(&mp_module_ucode) }, \
    { MP_ROM_QSTR(MP_QSTR_ucomponent), MP_ROM_PTR(&mp_module_ucomponent) }, \
    { MP_ROM_QSTR(MP_QSTR_ucomputer), MP_ROM_PTR(&mp_module_ucomputer) }, \
    { MP_ROM_QSTR(MP_QSTR_uos), MP_ROM_PTR(&mp_module_uos) }, \
    { MP_ROM_QSTR(MP_QSTR_uimp), MP_ROM_PTR(&mp_module_uimp) }, \
    { MP_ROM_QSTR(MP_QSTR_utime), MP_ROM_PTR(&mp_module_utime) }, \
    { MP_ROM_QSTR(MP_QSTR_umsgpack), MP_ROM_PTR(&mp_module_umsgpack) }, \
    { MP_ROM_QSTR(MP_QSTR_uvalue), MP_ROM_PTR(&mp_module_uvalue) }, \

#define MICROPY_PORT_BUILTIN_MODULE_WEAK_LINKS

#if 0
#endif
