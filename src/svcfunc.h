#ifndef OPENPIE_INCLUDED_SVCFUNC_H
#define OPENPIE_INCLUDED_SVCFUNC_H
#include "py/obj.h"

#define SVC_FUNC MP_NOINLINE

SVC_FUNC void svc_test(int a, int b, int c, int d);
SVC_FUNC mp_obj_t svc_syscall(char const *buf, mp_uint_t len);

#endif