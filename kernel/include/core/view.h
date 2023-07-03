/** !
 * GlucOS framebuffer view header
 * 
 * @file include/core/view.h
 * 
 * @author Jacob Smith
*/

// Include guard
#ifndef VIEW
#define VIEW

// Include
#include <core/gtypes.h>

// Defines
#define G_SUCCESS 1

// Constructor
int view_construct ( view *p_view, framebuffer *p_framebuffer, size_t abs_x, size_t abs_y, size_t width, size_t height );

int view_put_pixel ( view *p_view, size_t x, size_t y, pixel p );

int view_clear ( view *p_view, pixel p );

#endif