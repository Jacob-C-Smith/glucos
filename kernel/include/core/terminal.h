/** !
 * GlucOS terminal header
 * 
 * @file include/core/terminal.h
 * 
 * @author Jacob Smith
*/

// Include guard
#ifndef TERMINAL
#define TERMINAL

// Include
#include <core/gtypes.h>
#include <core/view.h>

int terminal_construct ( terminal *p_terminal );
int terminal_clear ( terminal *p_terminal );
int terminal_putchar ( terminal *p_terminal, char c );
int terminal_write ( terminal *p_terminal, char *text, size_t len );

#endif