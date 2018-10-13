#include "py/obj.h"
#include "py/runtime.h"
#include "syscall.h"
#include "msgpack.h"


mp_obj_t wrap_result(int retcode) {
    void *ret = (void *) retcode;
    if (ret == NULL) {
        return mp_const_none;
    } else {
        syscall_result_t *result = (syscall_result_t *) ret;
        int status = result->status;
        switch (status) {
            case 0: // ERROR
            {
                mp_obj_t args_obj = msgpack_loads((const char *)result->buf, result->len);

                size_t count = 0;
                mp_obj_t *items = NULL;
                mp_obj_get_array(args_obj, &count, &items);

                mp_obj_t exc = mp_obj_new_exception_args(&mp_type_SystemError, count, items);
                mp_obj_exception_add_traceback(exc, MP_QSTR__lt_ucomputer_gt_, 0, MP_QSTR__lt_syscall_gt_);
                nlr_raise(exc);
            }
            case 1: // OK (msgpack)
                return msgpack_loads((const char *)result->buf, result->len);
            case 2: // OK (int)
                // return msgpack_loads((const char *)result->buf, result->len);
                return mp_const_none; // NIY
            default:
            {
                mp_obj_t exc = mp_obj_new_exception_arg1(&mp_type_SystemError,
                    MP_OBJ_NEW_QSTR(MP_QSTR_invalid_code));
                nlr_raise(exc);
            }
        }
    }
}
