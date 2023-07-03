/** !
 * GlucOS kernel types
 * 
 * @file include/core/gtypes.h
 * 
 * @author Jacob Smith
*/

// Include guard
#ifndef GTYPES
#define GTYPES

// Type definitions

// Integers
typedef unsigned char      u8;
typedef          char      i8;
typedef signed   char      s8;
typedef unsigned short     u16;
typedef          short     i16;
typedef signed   short     s16;
typedef unsigned int       u32;
typedef          int       i32;
typedef signed   int       s32;
typedef unsigned long long u64;
typedef          long long i64;
typedef signed   long long s64;

// Unit size
typedef unsigned long long size_t;

// Floating 
typedef float  f32;
typedef double f64;

// Vectors
typedef struct { float x, y; }        vec2;
typedef struct { float x, y, z; }     vec3;
typedef struct { float x, y, z, w; }  vec4;
typedef struct { signed x, y; }       ivec2;
typedef struct { signed x, y, z; }    ivec3;
typedef struct { signed x, y, z, w; } ivec4;

// Matrices
typedef struct 
{ 
    float a, b, c, d;
} mat2;
typedef struct 
{ 
    float a, b, c, d, e, f, g, h, i;
} mat3;
typedef struct 
{ 
    float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
} mat4;

typedef struct kernel_s      kernel;
typedef struct framebuffer_s framebuffer;
typedef struct pixel_s       pixel;
typedef struct view_s        view;
typedef struct canvas_s      canvas;
typedef struct terminal_s    terminal;
typedef struct string_s      string;

// Framebuffer
struct framebuffer_s
{
	void               *p_base;
	unsigned long long  size;
	unsigned int        width;
	unsigned int        height;
	unsigned int        pixels_per_scanline;
};

// Pixel
struct pixel_s
{
    u8 b, g, r, a;
};

// Framebuffer view
struct view_s {
    framebuffer *p_framebuffer;
    size_t       abs_x,
                 abs_y,
                 width,
                 height;
};

// Canvas
struct canvas_s;

// Terminal
struct terminal_s {
    view   _view;
    size_t rows,
           columns,
           row,
           column,
           size;
    pixel  foreground,
           background;
};

// Kernel
struct kernel_s {
    struct {
        framebuffer *p_framebuffer;
        terminal     _terminal;
    } context;
    
    void *p_rsdp;
};



// String
struct string_s;

// List

// Array

// Queue

// Functions

/** ! 
 * 
 * 
 * 
 */

#endif