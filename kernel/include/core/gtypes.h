/** !
 * GlucOS kernel types
 * 
 * @file include/core/gtypes.h
 * 
 * @author Jacob Smith
*/

// Include guard
#pragma once

// Type definitions

// Integers
typedef unsigned char      u8;
typedef          char      i8;
typedef signed   char      s8;
typedef unsigned short     u16;
typedef          short     i16;
typedef signed   short     s16;
typedef unsigned long      u32;
typedef          long      i32;
typedef signed   long      s32;
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

// Functions

/** ! 
 * 
 * 
 * 
 */