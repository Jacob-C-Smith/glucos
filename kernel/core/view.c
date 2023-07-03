#include <core/view.h>

int view_construct ( view *p_view, framebuffer *p_framebuffer, size_t abs_x, size_t abs_y, size_t width, size_t height )
{

    // Argument check
    {
        #ifndef G_DEBUG
            if ( p_view == (void *) 0 )
                return 0;
            if ( p_framebuffer == (void *) 0 )
                return 0;
        #endif
    }

    *p_view = (view) {
        .p_framebuffer = p_framebuffer,
        .abs_x         = abs_x,
        .abs_y         = abs_y,
        .width         = width,
        .height        = height
    };

    // Success
    return G_SUCCESS;
}

int view_put_pixel ( view *p_view, size_t x, size_t y, pixel p )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_view == (void *) 0 )
                goto no_view;
        #endif
    }

    // Initialized data
    framebuffer *p_framebuffer = p_view->p_framebuffer;
    u32         *fbd           = p_framebuffer->p_base;
    
    if ( x > p_view->width-1 || y > p_view->height-1 )
        return 0;

    fbd = &fbd[(p_view->abs_y+y) * p_framebuffer->width + (p_view->abs_x+x)];

    *(pixel *)fbd = p;

    // Success
    return G_SUCCESS;

    // Error handling
    {

        // Argument errors
        {
            no_view:
                return 0;
        }
    }
}

int view_clear ( view *p_view, pixel p )
{
    for (size_t y = 0; y < p_view->height; y++)
    {
        for (size_t x = 0; x < p_view->width; x++)
        {
            view_put_pixel(p_view, x, y, p);
        }
    }
    
    return 1;
}