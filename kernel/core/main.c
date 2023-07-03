/** !
 * @author Jacob Smith
 * 
 * @file kernel/core/kernel.c
 * 
 * Kernel core
*/

#include <core/g.h>
#include <core/gtypes.h>
#include <core/view.h>
#include <core/terminal.h>
#include <string.h>

int main ( kernel *p_kernel );

int pixel_put ( int x, int y, pixel p )
{
    kernel      *p_kernel      = get_active_kernel();
    framebuffer *p_framebuffer = p_kernel->context.p_framebuffer;

    u32 *fbd = p_framebuffer->p_base;
    fbd = &fbd[y * p_framebuffer->width + x];

    *(pixel *)fbd = p;

    return G_SUCCESS;
}

/*
void draw_char ( int x, int y, int size, char c )
{
    // Initialized data
    u64 glyph = font[(u8)c];

    // Iterate over the width and the height of the glyph
    for (s32 h = (y + (8 * size)); h > y; h -= size)
        for (s32 w = x; w < (x + (8 * size)); w += size)
        {

            // Draw a pixel in the glyph
            if (glyph & 1)

                // Of a specified size
                for (s32 i = 0; i < size; i++)
                    for (s32 j = 0; j < size; j++)

                        // Draw the pixel
                        pixel_put(w + i, h + j,(pixel) { .r = 0x00, .g = 0x00, .b = 0x00, .a = 0xff });

            // Draw the next part of the glyph
            glyph >>= 1;
        }

    // Return
    return;
}

int draw_text ( int x, int y, int size, char *str )
{
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++)
        draw_char(x+(i*(size*8)), y, size, str[i]);
    
    return G_SUCCESS;
}
*/

int _start ( framebuffer *p_framebuffer, void *p_rsdp )
{

    // External data
    extern kernel active_kernel;

    // Construct the kernel
    active_kernel = (kernel)
    {
        .context =
        {
            .p_framebuffer = p_framebuffer
        },
        .p_rsdp = p_rsdp
    };
    
    terminal_construct( &active_kernel.context._terminal );

    // Kernel main
    main( get_active_kernel() );

    // Success
    return G_SUCCESS;
}

int clear_screen ( framebuffer *p_framebuffer)
{
    for (size_t y = 0; y < p_framebuffer->height; y++)
    {
        for (size_t x = 0; x < p_framebuffer->width; x++)
        {
            pixel_put(x, y,(pixel) { .r = 0x80, .g = 0x80, .b = 0xff, .a = 0xff });
        }
    }
    
    return 1;
}

int clear_screen_color ( framebuffer *p_framebuffer )
{

    for (size_t y = 0; y < p_framebuffer->height; y++)
    {
        for (size_t x = 0; x < p_framebuffer->width; x++)
        {
            double r = ((double)y/(double)p_framebuffer->height)*255;
            double g = ((double)x/(double)p_framebuffer->width)*255;

            pixel px = { .r = r, .g = g, .b = ((r+g)/2), .a = 0x00 };

            done:
            pixel_put(x, y, px);
        }
    }
    
    return 1;
}

int main ( kernel *p_kernel )
{
    char *z = "GlucOS ";
    
    terminal_clear(&p_kernel->context._terminal);
    terminal_write(&p_kernel->context._terminal, z, strlen(z));
   
    return G_SUCCESS;
}