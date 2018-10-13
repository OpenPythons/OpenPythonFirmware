#include <string.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/gc.h"
#include "py/runtime.h"
#include "lib/mpack/mpack.h"
#include "syscall.h"
#include "msgpack.h"
#include "uvalue.h"

mp_obj_t oc_create_value(mp_obj_t raw_value_obj);

mp_obj_t msgpack_load(mpack_reader_t *reader) {
    mpack_tag_t tag = mpack_read_tag(reader);
    mp_obj_t obj = NULL;

    switch (tag.type) {
        case mpack_type_nil:
            obj = mp_const_none;

            mpack_done_type(reader, tag.type);
            break;
        case mpack_type_bool:
            if (tag.v.b)
                obj = mp_const_true;
            else
                obj = mp_const_false;

            mpack_done_type(reader, tag.type);
            break;
        case mpack_type_int:
        {
            int64_t i64 = tag.v.i;
            mp_int_t i32 = (mp_int_t)i64 + 0;

            if (i32 == i64)
                obj = mp_obj_new_int(i32);
            else
                obj = mp_obj_new_int_from_ll(tag.v.i);

            mpack_done_type(reader, tag.type);
            break;
        }
        case mpack_type_uint:
        {
            uint64_t u64 = tag.v.i;
            mp_uint_t u32 = (mp_int_t)u64 + 0;

            if (u32 == u64)
                obj = mp_obj_new_int_from_uint(u64);
            else
                obj = mp_obj_new_int_from_ull(u64);

            mpack_done_type(reader, tag.type);
            break;
        }
        case mpack_type_float:
            obj = mp_obj_new_float(tag.v.f);

            mpack_done_type(reader, tag.type);
            break;
        case mpack_type_double:
            obj = mp_obj_new_float(tag.v.d);

            mpack_done_type(reader, tag.type);
            break;
        case mpack_type_str:
        {
            uint32_t length = tag.v.l;
            const char *buf = mpack_read_utf8_inplace(reader, length);

            obj = mp_obj_new_str(buf, length);

            mpack_done_str(reader);
            break;
        }
        case mpack_type_bin:
        {
            uint32_t length = tag.v.l;
            const char *buf = mpack_read_bytes_inplace(reader, length);

            obj = mp_obj_new_bytes((const byte *)buf, length);

            mpack_done_bin(reader);
            break;
        }
        case mpack_type_array:
        {
            uint32_t count = tag.v.n;
            mp_obj_tuple_t *t = mp_obj_new_tuple(count, NULL);
            for (uint32_t i = 0; i < count; i++)
                t->items[i] = msgpack_load(reader);

            obj = t;
            mpack_done_array(reader);
            break;
        }
        case mpack_type_map:
        {
            uint32_t count = tag.v.n;
            obj = mp_obj_new_dict(count);
            for (uint32_t i = 0; i < count; i++) {
                mp_obj_t key = msgpack_load(reader);
                mp_obj_t value = msgpack_load(reader);
                mp_obj_dict_store(obj, key, value);
            }

            mpack_done_array(reader);
            break;
        }
        case mpack_type_ext:
        {
            int8_t exttype = tag.exttype;
            uint32_t length = tag.v.l;

            switch (exttype) {
                case 1: // OpenComputer.Value
                {
                    const char *buf = mpack_read_bytes_inplace(reader, length);
                    obj = msgpack_loads(buf, length);
                    obj = oc_create_value(obj);
                    break;
                }
                default:
                    mp_raise_ValueError("invalid ext");
            }

            mpack_done_ext(reader);
            break;
        }
        default:
            mp_raise_ValueError("invalid tag");
    }

    mpack_assert(obj != NULL);
    return obj;
}

mpack_reader_t *msgpack_load_new(const char *data, size_t size) {
    mpack_reader_t *reader = (mpack_reader_t *) MPACK_MALLOC(sizeof(mpack_reader_t));
    mpack_reader_init_data(reader, data, size);
    return reader;
}

mp_obj_t msgpack_loads(const char *buf, size_t len) {
    mpack_reader_t *reader = msgpack_load_new(buf, len);
    mp_obj_t result = msgpack_load(reader);
    size_t remaining = mpack_reader_remaining(reader, &buf);
    if (remaining != 0)
        mp_raise_ValueError("msgpack: remaining != 0");

    return result;
}

void msgpack_dump(mpack_writer_t *writer, mp_obj_t obj) {
#define CHECK(type) (MP_OBJ_IS_TYPE(obj, &type))
    if (obj == mp_const_none) {
        mpack_write_nil(writer);
    } else if (obj == mp_const_true) {
        mpack_write_bool(writer, true);
    } else if (obj == mp_const_false) {
        mpack_write_bool(writer, false);
    } else if (MP_OBJ_IS_SMALL_INT(obj)) {
        int value = MP_OBJ_SMALL_INT_VALUE(obj);
        mpack_write_int(writer, value);
    } else if (MP_OBJ_IS_QSTR(obj)) {
        size_t len;
        const char *str = mp_obj_str_get_data(obj, &len);
        mpack_write_str(writer, str, len);
    } else if (MP_OBJ_IS_FUN(obj)) { // function (error)
        mp_raise_TypeError(NULL);
    } else if (CHECK(mp_type_int)) {
        int value = mp_obj_get_int_truncated(obj);
        mpack_write_int(writer, value);
    } else if (CHECK(mp_type_float)) {
        mp_float_t value = mp_obj_float_get(obj);
#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
        mpack_write_float(writer, value);
#elif MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
        mpack_write_double(writer, value);
#else
    #if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_NONE
        #error MICROPY_FLOAT_IMPL_NONE
    #else
        #error MICROPY_FLOAT_IMPL mismatch
    #endif
#endif
    } else if (CHECK(mp_type_str)) {
        size_t len;
        const char *str = mp_obj_str_get_data(obj, &len);
        mpack_write_str(writer, str, len);
    } else if (CHECK(mp_type_bytes)) {
        size_t len;
        const char *str = mp_obj_str_get_data(obj, &len);
        mpack_write_bin(writer, str, len);
    } else if (CHECK(mp_type_list) || CHECK(mp_type_tuple)) {
        size_t len;
        mp_obj_t *items;
        mp_obj_get_array(obj, &len, &items);
        mpack_start_array(writer, len);
        for (size_t i = 0; i < len; i++) {
            msgpack_dump(writer, items[i]);
        }
        mpack_finish_array(writer);
    } else if (CHECK(mp_type_dict)) {
        mp_map_t *map = mp_obj_dict_get_map(obj);
        size_t max = map->alloc;
        mpack_start_map(writer, map->used);
        for (size_t i = 0; i < max; i++) {
            if (MP_MAP_SLOT_IS_FILLED(map, i)) {
                mp_map_elem_t *item = &(map->table[i]);
                msgpack_dump(writer, item->key);
                msgpack_dump(writer, item->value);
            }
        }
        mpack_finish_map(writer);
    } else if (CHECK(uvalue_type)) {
        uvalue_obj_t *uvalue = (uvalue_obj_t *)obj;
        size_t size = 0;
        byte *buffer = NULL;

        msgpack_dumps(uvalue->value, &buffer, &size);

        int UVALUE_TYPE = 1;
        mpack_write_ext(writer, UVALUE_TYPE, (const char *)buffer, size);
    } else {
        mp_raise_TypeError(NULL);
    }
}

mpack_writer_t *msgpack_dump_new(byte **data, size_t *size) {
    mpack_writer_t *writer = (mpack_writer_t *) MPACK_MALLOC(sizeof(mpack_writer_t));
    mpack_writer_init_growable(writer, (char **) data, size);
    return writer;
}

void msgpack_dump_close(mpack_writer_t *writer) {
    mpack_error_t error = mpack_writer_destroy(writer);
    switch (error) {
        case mpack_ok:
            return;
        default:
            // TODO: ?
            mp_raise_ValueError("msgpack check fail");
    }
}

void msgpack_dumps(mp_obj_t obj, byte **data, size_t *size) {
    mpack_writer_t *writer = msgpack_dump_new(data, size);
    msgpack_dump(writer, obj);
    msgpack_dump_close(writer);
}
