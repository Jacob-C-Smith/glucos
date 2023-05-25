/** !
 * @author Jacob Smith
 * 
 * @file kernel/core/string.c
 * 
 * String 
*/

// Include
#include <string.h>

size_t string_length ( char *str )
{
    
    // Argument check
    {
        #ifndef G_DEBUG
            if ( str == (void *) 0 )
                goto no_str;
        #endif
    }
    
    // Initialized data
    size_t len = 0;

    // TODO: Compute the length of the string

    // Return the length of the string
    return len;

    // Error handling
    {
        
        // Argument errors
        {
            no_str:
                #ifndef GDEBUG
                    // TODO: Error message
                #endif

                // Error
                return 0;
        }
    }
}