#include <stdio.h>
#include <direct.h>

char *
compat_getcwd(char *buffer, int maxlen)
{
    char *bp;

    if (getcwd(buffer, maxlen) == (void *) 0) {
	return NULL;
    }

    for (bp = buffer; *bp != '\0'; bp++) {
	if (*bp == '\\') {
	    *bp = '/';
	}
    }
    return buffer;
}
