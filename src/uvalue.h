#include <py/obj.h>

extern const mp_obj_type_t uvalue_type;

typedef struct _uvalue_obj_t {
    mp_obj_base_t base;
    mp_obj_t value;
} uvalue_obj_t;
