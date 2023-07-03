#include <core/g.h>

kernel active_kernel = {
    .context = {
        .p_framebuffer = 0,
    },
    .p_rsdp = 0
};

kernel *get_active_kernel ( void )
{
    return &active_kernel;
}

