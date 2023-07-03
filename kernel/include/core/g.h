/** !
 * GlucOS kernel header
 * 
 * @file include/core/g.h
 * 
 * @author Jacob Smith
*/

// Include guard
#ifndef G
#define G

// Include
#include <core/gtypes.h>
#include <core/terminal.h>

// Defines
#define G_SUCCESS 1

// Structures


/** !
 * Get a reference to the kernel
 * 
 * @param void
 * 
 * @return pointer to kernel struct
*/
kernel *get_active_kernel ( void );

/** !
 * (re)allocate memory
 * 
 * @param ptr  : void *
 * @param size : size_t 
 * 
*/
void *realloc ( void *ptr, size_t size );

#endif