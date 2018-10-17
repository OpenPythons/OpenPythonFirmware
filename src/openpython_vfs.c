/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2018 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "py/runtime.h"
#include "py/stream.h"
#include "py/mperrno.h"
#include "openpython_vfs.h"
#include "openpython_vfs.h"
#include "syscall.h"


#if MICROPY_OPENPYTHON_VFS


typedef struct _mp_obj_vfs_openpython_file_t {
    mp_obj_base_t base;
    int fd;
} mp_obj_vfs_openpython_file_t;

STATIC void check_fd_is_open(const mp_obj_vfs_openpython_file_t *o) {
    if (o->fd == -1) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "I/O operation on closed file"));
    }
}

STATIC void vfs_openpython_file_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    // SVC_FUNCTION_START;
    (void) kind;
    mp_obj_vfs_openpython_file_t *self = MP_OBJ_TO_PTR(self_in);

    mp_printf(print, "<io.%s fd=%d>", mp_obj_get_type_str(self_in), self->fd);
}

mp_obj_t mp_vfs_openpython_file_open(const mp_obj_type_t *type, mp_obj_t address_in, mp_obj_t path_in, mp_obj_t mode_in) {
    mp_obj_vfs_openpython_file_t *o = m_new_obj(mp_obj_vfs_openpython_file_t);
    const char *address = mp_obj_str_get_str(address_in);
    const char *path = mp_obj_str_get_str(path_in);
    const char *mode = mp_obj_str_get_str(mode_in);

    o->base.type = type;
    o->fd = -1;

    int errno = __syscall4(SYS_VFS_OPEN, (int) address, (int) path, (int) mode, (int) &(o->fd));
    if (errno != 0) {
        mp_raise_OSError(errno);
    }

    check_fd_is_open(o);
    return MP_OBJ_FROM_PTR(o);
}

STATIC mp_obj_t vfs_openpython_file_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_address, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_path,    MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_mode,    MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
    };

    mp_arg_val_t arg_vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, arg_vals);
    return mp_vfs_openpython_file_open(type, arg_vals[0].u_obj, arg_vals[1].u_obj, arg_vals[2].u_obj);
}

STATIC mp_obj_t vfs_openpython_file_fileno(mp_obj_t self_in) {
    mp_obj_vfs_openpython_file_t *self = MP_OBJ_TO_PTR(self_in);
    check_fd_is_open(self);
    return mp_obj_new_int(self->fd);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(vfs_openpython_file_fileno_obj, vfs_openpython_file_fileno);

STATIC mp_obj_t vfs_openpython_file___exit__(size_t n_args, const mp_obj_t *args) {
    (void) n_args;
    return mp_stream_close(args[0]);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(vfs_openpython_file___exit___obj, 4, 4, vfs_openpython_file___exit__);

STATIC mp_uint_t vfs_openpython_file_read(mp_obj_t o_in, void *buf, mp_uint_t size, int *errcode) {
    mp_obj_vfs_openpython_file_t *o = MP_OBJ_TO_PTR(o_in);
    check_fd_is_open(o);

    int count = 0;
    int errno = __syscall4(SYS_VFS_READ, o->fd, (int) size, (int) buf, (int) &count);
    if (errno != 0) {
        *errcode = errno;
        return MP_STREAM_ERROR;
    }
    return count;
}

STATIC mp_uint_t vfs_openpython_file_write(mp_obj_t o_in, const void *buf, mp_uint_t size, int *errcode) {
    mp_obj_vfs_openpython_file_t *o = MP_OBJ_TO_PTR(o_in);
    check_fd_is_open(o);
#if MICROPY_PY_OS_DUPTERM
    if (o->flag & 1) {
        switch (o->fd) {
            case STDIN_FILENO:
                return 0;
            case STDOUT_FILENO:
                // mp_hal_stdout_tx_strn(buf, size);
                mp_hal_stdio_tx_strn(MP_STATE_PORT(stdout_hook_obj), buf, size);
                return size;
            case STDERR_FILENO:
            {
                mp_obj_t stdio_hook_obj = MP_STATE_PORT(stderr_hook_obj);
                if (stdio_hook_obj == NULL)
                    stdio_hook_obj = MP_STATE_PORT(stdout_hook_obj);

                mp_hal_stdio_tx_strn(stdio_hook_obj, buf, size);
                return size;
            }
        }
    }
#endif
    int written = 0;
    int errno = __syscall4(SYS_VFS_WRITE, o->fd, (int) buf, (int) size, (int) &written);
    if (errno != 0) {
        *errcode = errno;
        return MP_STREAM_ERROR;
    }
    return (mp_uint_t) written;
}

STATIC mp_uint_t vfs_openpython_file_ioctl(mp_obj_t o_in, mp_uint_t request, uintptr_t arg, int *errcode) {
    mp_obj_vfs_openpython_file_t *o = MP_OBJ_TO_PTR(o_in);
    check_fd_is_open(o);
    int errno;
    switch (request) {
        case MP_STREAM_FLUSH:
            errno = __syscall3(SYS_VFS_FLUSH, o->fd, 0, 0);
            if (errno != 0) {
                *errcode = errno;
                return MP_STREAM_ERROR;
            }
            return 0;
        case MP_STREAM_SEEK: {
            struct mp_stream_seek_t *s = (struct mp_stream_seek_t *) arg;
            int offset = 0;
            errno = __syscall4(SYS_VFS_SEEK, o->fd, s->offset, s->whence, (int) &offset);
            if (errno != 0) {
                *errcode = errno;
                return MP_STREAM_ERROR;
            }
            s->offset = offset;
            return 0;
        }
        case MP_STREAM_CLOSE:
            errno = __syscall3(SYS_VFS_CLOSE, o->fd, 0, 0);
            if (errno != 0) {
                *errcode = errno;
                return MP_STREAM_ERROR;
            }
            o->fd = -1;
            return 0;
        default:
            *errcode = MP_EINVAL;
            return MP_STREAM_ERROR;
    }
}

STATIC const mp_rom_map_elem_t rawfile_locals_dict_table[] = {
        {MP_ROM_QSTR(MP_QSTR_fileno),    MP_ROM_PTR(&vfs_openpython_file_fileno_obj)},
        {MP_ROM_QSTR(MP_QSTR_read),      MP_ROM_PTR(&mp_stream_read_obj)},
        {MP_ROM_QSTR(MP_QSTR_readinto),  MP_ROM_PTR(&mp_stream_readinto_obj)},
        {MP_ROM_QSTR(MP_QSTR_readline),  MP_ROM_PTR(&mp_stream_unbuffered_readline_obj)},
        {MP_ROM_QSTR(MP_QSTR_readlines), MP_ROM_PTR(&mp_stream_unbuffered_readlines_obj)},
        {MP_ROM_QSTR(MP_QSTR_write),     MP_ROM_PTR(&mp_stream_write_obj)},
        {MP_ROM_QSTR(MP_QSTR_seek),      MP_ROM_PTR(&mp_stream_seek_obj)},
        {MP_ROM_QSTR(MP_QSTR_tell),      MP_ROM_PTR(&mp_stream_tell_obj)},
        {MP_ROM_QSTR(MP_QSTR_flush),     MP_ROM_PTR(&mp_stream_flush_obj)},
        {MP_ROM_QSTR(MP_QSTR_close),     MP_ROM_PTR(&mp_stream_close_obj)},
        {MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&mp_identity_obj)},
        {MP_ROM_QSTR(MP_QSTR___exit__),  MP_ROM_PTR(&vfs_openpython_file___exit___obj)},
};

STATIC MP_DEFINE_CONST_DICT(rawfile_locals_dict, rawfile_locals_dict_table);

#if MICROPY_PY_IO_FILEIO
STATIC const mp_stream_p_t fileio_stream_p = {
        .read = vfs_openpython_file_read,
        .write = vfs_openpython_file_write,
        .ioctl = vfs_openpython_file_ioctl,
};

const mp_obj_type_t mp_type_vfs_openpython_fileio = {
        {&mp_type_type},
        .name = MP_QSTR_FileIO,
        .print = vfs_openpython_file_print,
        .make_new = vfs_openpython_file_make_new,
        .getiter = mp_identity_getiter,
        .iternext = mp_stream_unbuffered_iter,
        .protocol = &fileio_stream_p,
        .locals_dict = (mp_obj_dict_t *) &rawfile_locals_dict,
};
#endif

STATIC const mp_stream_p_t textio_stream_p = {
        .read = vfs_openpython_file_read,
        .write = vfs_openpython_file_write,
        .ioctl = vfs_openpython_file_ioctl,
        .is_text = true,
};

const mp_obj_type_t mp_type_vfs_openpython_textio = {
        {&mp_type_type},
        .name = MP_QSTR_TextIOWrapper,
        .print = vfs_openpython_file_print,
        .make_new = vfs_openpython_file_make_new,
        .getiter = mp_identity_getiter,
        .iternext = mp_stream_unbuffered_iter,
        .protocol = &textio_stream_p,
        .locals_dict = (mp_obj_dict_t *) &rawfile_locals_dict,
};

#endif // MICROPY_OPENPYTHON_VFS
