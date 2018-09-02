#ifndef OPENPIE_UMSGPACK_H
#define OPENPIE_UMSGPACK_H

#include "py/obj.h"
#include "lib/mpack/mpack.h"

mp_obj_t umsgpack_loads(const char *buf, size_t len);

const char *umsgpack_dumps(mp_obj_t obj);

#endif // OPENPIE_UMSGPACK_H
