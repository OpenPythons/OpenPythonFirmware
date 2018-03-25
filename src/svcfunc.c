#include <stdio.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "svc.h"
#include "svcfunc.h"


SVC_FUNC void svc_test(int a, int b, int c, int d)
{
    SVC_FUNCTION_START;

    SVC4(0, a, b, c, d);

    printf("!! %d %d %d %d\n", R0, R1, R2, R3);
}


SVC_FUNC mp_obj_t svc_syscall(const char *buf, mp_uint_t len)
{
    SVC_FUNCTION_START;

    SVC2(1, buf, len);

    return mp_obj_new_str_copy(&mp_type_str, (const byte *)R0, (size_t)R1);
}
