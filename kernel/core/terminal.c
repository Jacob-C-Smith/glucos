// Each u64 encodes an 8x8 glyph. ASCII values can be used to index characters. Add whatever you want so long as you keep the length under 255
unsigned long long font[136] = {
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x183C3C1818001800, 0x3636000000000000, 0x36367F367F363600, 0x0C3E031E301F0C00, 0x006333180C666300, 0x1C361C6E3B336E00, 0x0606030000000000,
    0x180C0606060C1800, 0x060C1818180C0600, 0x00663CFF3C660000, 0x000C0C3F0C0C0000, 0x00000000000C0C06, 0x0000003F00000000, 0x00000000000C0C00, 0x6030180C06030100,
    0x3E63737B6F673E00, 0x0C0E0C0C0C0C3F00, 0x1E33301C06333F00, 0x1E33301C30331E00, 0x383C36337F307800, 0x3F031F3030331E00, 0x1C06031F33331E00, 0x3F3330180C0C0C00,
    0x1E33331E33331E00, 0x1E33333E30180E00, 0x000C0C00000C0C00, 0x000C0C00000C0C06, 0x180C0603060C1800, 0x00003F00003F0000, 0x060C1830180C0600, 0x1E3330180C000C00,
    0x3E637B7B7B031E00, 0x0C1E33333F333300, 0x3F66663E66663F00, 0x3C66030303663C00, 0x1F36666666361F00, 0x7F46161E16467F00, 0x7F46161E16060F00, 0x3C66030373667C00,
    0x3333333F33333300, 0x1E0C0C0C0C0C1E00, 0x7830303033331E00, 0x6766361E36666700, 0x0F06060646667F00, 0x63777F7F6B636300, 0x63676F7B73636300, 0x1C36636363361C00,
    0x3F66663E06060F00, 0x1E3333333B1E3800, 0x3F66663E36666700, 0x1E33070E38331E00, 0x3F2D0C0C0C0C1E00, 0x3333333333333F00, 0x33333333331E0C00, 0x6363636B7F776300,
    0x6363361C1C366300, 0x3333331E0C0C1E00, 0x7F6331184C667F00, 0x1E06060606061E00, 0x03060C1830604000, 0x1E18181818181E00, 0x081C366300000000, 0x00000000000000FF,
    0x0C0C180000000000, 0x00001E303E336E00, 0x0706063E66663B00, 0x00001E3303331E00, 0x3830303e33336E00, 0x00001E333f031E00, 0x1C36060f06060F00, 0x00006E33333E301F,
    0x0706366E66666700, 0x0C000E0C0C0C1E00, 0x300030303033331E, 0x070666361E366700, 0x0E0C0C0C0C0C1E00, 0x0000337F7F6B6300, 0x00001F3333333300, 0x00001E3333331E00,
    0x00003B66663E060F, 0x00006E33333E3078, 0x00003B6E66060F00, 0x00003E031E301F00, 0x080C3E0C0C2C1800, 0x0000333333336E00, 0x00003333331E0C00, 0x0000636B7F7F3600,
    0x000063361C366300, 0x00003333333E301F, 0x00003F190C263F00, 0x380C0C070C0C3800, 0x1818180018181800, 0x070C0C380C0C0700, 0x6E3B000000000000, 0x0000000000000000,
    0x80C0E070371E1C08, 0x00001c3e3e3e1c00, 0x040C1C3C3C1C0C04, 0x0000ff4224180000, 0x7E7E7E7E7E7E3C18, 0x0709FF81818181FF, 0x784C46424242427E, 0x183C7E1818181818
};

#include <core/terminal.h>

int terminal_construct ( terminal *p_terminal )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_terminal == (void *) 0 ) goto no_terminal;
        #endif
    }

    // Initialized data
    kernel *p_kernel = get_active_kernel();

    *p_terminal = (terminal)
    {
        .columns = 624/8,
        .rows = 480/8,
        .row = 0,
        .column = 0,
        ._view = { 0 },
        .foreground = (pixel) { .r = 0x00, .g = 0x00, .b = 0x00, .a = 0xff },
        .background = (pixel) { .r = 0xff, .g = 0xff, .b = 0xff, .a = 0x00 },
        .size = 1
    };

    view_construct(&p_terminal->_view, p_kernel->context.p_framebuffer, 640, 480, 624, 480);

    // Success
    return G_SUCCESS;

    // Error handling
    {

        // Argument errors
        {
            no_terminal:
                return 0;
        }
    }
}

int terminal_putcharat ( terminal *p_terminal, char c, size_t x, size_t y )
{
    // Initialized data
    u64 glyph = font[(u8)c];

    // Iterate over the width and the height of the glyph
    for (s32 h = (y + (8 * p_terminal->size)); h > y; h -= p_terminal->size)
        for (s32 w = x; w < (x + (8 * p_terminal->size)); w += p_terminal->size)
        {

            // Draw a pixel in the glyph
            if (glyph & 1)
            {
                // Of a specified size
                for (s32 i = 0; i < p_terminal->size; i++)
                    for (s32 j = 0; j < p_terminal->size; j++)

                        // Draw the pixel
                        view_put_pixel(&p_terminal->_view,w + i, h + j,p_terminal->foreground);
            }
            else
            {
                // Of a specified size
                for (s32 i = 0; i < p_terminal->size; i++)
                    for (s32 j = 0; j < p_terminal->size; j++)

                        // Draw the pixel
                        view_put_pixel(&p_terminal->_view,w + i, h + j,p_terminal->background);
            }

            // Draw the next part of the glyph
            glyph >>= 1;
        }

    // Return
    return;
}

int terminal_putchar ( terminal *p_terminal, char c )
{
    // Initialized data
    u64 glyph = font[(u8)c];

    terminal_putcharat(p_terminal,c,  p_terminal->column*8*p_terminal->size, p_terminal->row*8*p_terminal->size);
    p_terminal->column++;
   
    if(p_terminal->column > p_terminal->columns-2)
    {
        p_terminal->column=0,
        p_terminal->row++;
    }

    // Return
    return G_SUCCESS;
}

int terminal_clear ( terminal *p_terminal )
{
    view_clear(&p_terminal->_view,p_terminal->background);
    return G_SUCCESS;
}

int terminal_write ( terminal *p_terminal, char *text, size_t len )
{   
    for (size_t i = 0; i < len; i++)
    {
        switch (text[i])
        {
            
            case '\n':
                p_terminal->column = 0, p_terminal->row++;
                break;

            case '\r':
                p_terminal->column = 0;
                break;
            /*
            case '\f':

                break;

            case '\t':

                break;

            case '\b':

                break;
            */
            default:
                terminal_putchar(p_terminal,text[i]);
                break;
        }
    }
    
    return G_SUCCESS;
}