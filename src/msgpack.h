#ifndef OPENPIE_UMSGPACK_H
#define OPENPIE_UMSGPACK_H

#include "py/obj.h"
#include "lib/mpack/mpack.h"

mp_obj_t msgpack_loads(const char *buf, size_t len);

mpack_writer_t *msgpack_dump_new(byte **data, size_t *size);
bool msgpack_dump(mpack_writer_t *writer, mp_obj_t obj);
void msgpack_dump_close(mpack_writer_t *writer);
void msgpack_dumps(mp_obj_t obj, byte **data, size_t *size);

#endif // OPENPIE_UMSGPACK_H
