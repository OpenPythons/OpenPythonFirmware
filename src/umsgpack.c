#include "py/obj.h"
#include "py/objstr.h"
#include "py/gc.h"
#include "lib/mpack/mpack.h"


mp_obj_t umsgpack_loads(const char *buf, size_t len) {
    return mp_const_none;
}

bool umsgpack_dumps_visit(mpack_writer_t *writer, mp_obj_t obj) {
#define CHECK(type) (MP_OBJ_IS_TYPE(obj, &type))
    if (MP_OBJ_IS_SMALL_INT(obj)) {
        int value = MP_OBJ_SMALL_INT_VALUE(obj);
        mpack_write_int(writer, value);
    } else if (MP_OBJ_IS_QSTR(obj)) {
        size_t len;
        const char *str = mp_obj_str_get_data(obj, &len);
        mpack_write_str(writer, str, len);
    } else if (MP_OBJ_IS_FUN(obj)) { // function (error)
        return false;
    } else if (CHECK(mp_type_int)) {
        int value = MP_OBJ_SMALL_INT_VALUE(obj);
        mpack_write_int(writer, value);
#if MICROPY_FLOAT_IMPL != MICROPY_FLOAT_IMPL_NONE
    } else if (CHECK(mp_type_float)) {
        mp_float_t value = mp_obj_float_get(obj);

#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
        mpack_write_float(writer, value);
#elif MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
        mpack_write_double(writer, value);
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
    } else if (CHECK(mp_type_bytes) || CHECK(mp_type_bytes)) {

    } else if (CHECK(mp_type_bytes)) {

    } else {
        return false;
    }

    return true;
}

mpack_writer_t *umsgpack_dump(mp_obj_t obj) {
    char *data;
    size_t size;
    mpack_writer_t *writer = (mpack_writer_t *) MPACK_MALLOC(sizeof(mpack_writer_t));
    mpack_writer_init_growable(writer, &data, &size);
    bool result = umsgpack_dumps_visit(writer, obj);
    if (!result) {
        // raise exception?
        return NULL;
    }

    return writer;
}
