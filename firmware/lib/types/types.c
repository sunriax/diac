
#include "types.h"

unsigned char types_check_isnumeric(const char *s)
{
    while (*s) {
        if(*s == '\0')
        {
            return 1;
        }
        else if (isdigit(*s++) == 0)
        {
            return 0;
        }
    }
    return 1;
}