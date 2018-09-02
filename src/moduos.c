#include "py/obj.h"
#include "extmod/vfs.h"

STATIC const mp_rom_map_elem_t uos_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_uos)},

        {MP_ROM_QSTR(MP_QSTR_mount),    MP_ROM_PTR(&mp_vfs_mount_obj)},
        {MP_ROM_QSTR(MP_QSTR_umount),   MP_ROM_PTR(&mp_vfs_umount_obj)},
        {MP_ROM_QSTR(MP_QSTR_chdir),    MP_ROM_PTR(&mp_vfs_chdir_obj)},
        {MP_ROM_QSTR(MP_QSTR_getcwd),   MP_ROM_PTR(&mp_vfs_getcwd_obj)},
        {MP_ROM_QSTR(MP_QSTR_ilistdir), MP_ROM_PTR(&mp_vfs_ilistdir_obj)},
        {MP_ROM_QSTR(MP_QSTR_listdir),  MP_ROM_PTR(&mp_vfs_listdir_obj)},
        {MP_ROM_QSTR(MP_QSTR_mkdir),    MP_ROM_PTR(&mp_vfs_mkdir_obj)},
        {MP_ROM_QSTR(MP_QSTR_remove),   MP_ROM_PTR(&mp_vfs_remove_obj)},
        {MP_ROM_QSTR(MP_QSTR_rename),   MP_ROM_PTR(&mp_vfs_rename_obj)},
        {MP_ROM_QSTR(MP_QSTR_rmdir),    MP_ROM_PTR(&mp_vfs_rmdir_obj)},
        {MP_ROM_QSTR(MP_QSTR_stat),     MP_ROM_PTR(&mp_vfs_stat_obj)},
        {MP_ROM_QSTR(MP_QSTR_statvfs),  MP_ROM_PTR(&mp_vfs_statvfs_obj)},
};

STATIC MP_DEFINE_CONST_DICT(uos_module_globals, uos_module_globals_table);

const mp_obj_module_t mp_module_uos = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &uos_module_globals,
};
