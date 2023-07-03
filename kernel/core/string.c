/** !
 * @author Jacob Smith
 * 
 * @file kernel/core/string.c
 * 
 * String 
*/

// Include
#include <string.h>

/*
struct string_s {
    char    *str;
    size_t   len;
    u64     *hash;
    int    (*copy)(string *p_str, string **pp_str);
};*/

size_t strlen ( char *p_str )
{
    
    // Argument check
    {
        #ifndef G_DEBUG
            if ( p_str == (void *) 0 )
                goto no_str;
        #endif
    }
    
    // Initialized data
    size_t len = 0;

    // Compute the length of the string
    while(p_str[++len]);

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