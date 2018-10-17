#ifndef OPENPYTHON_UMSGPACK_H
#define OPENPYTHON_UMSGPACK_H

#include "py/obj.h"
#include "lib/mpack/mpack.h"

typedef struct _msgpack_result_t {
    byte *data;
    size_t size;
} msgpack_result_t;


mp_obj_t msgpack_loads(const char *buf, size_t len);

mpack_writer_t *msgpack_dump_new(byte **data, size_t *size);
void msgpack_dump(mpack_writer_t *writer, mp_obj_t obj);
void msgpack_dump_close(mpack_writer_t *writer);

msgpack_result_t msgpack_dumps(mp_obj_t obj);
msgpack_result_t msgpack_args_dumps(size_t n_args, const mp_obj_t *args);

#endif // OPENPYTHON_UMSGPACK_H
