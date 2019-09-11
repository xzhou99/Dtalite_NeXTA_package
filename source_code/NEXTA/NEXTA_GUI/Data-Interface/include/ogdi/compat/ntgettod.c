#include <winsock.h>
#include <sys/timeb.h>

void _cdecl
gettimeofday(struct timeval *tv, struct timezone *tz)
{                                
    struct timeb tb;

    ftime(&tb);
    tv->tv_sec = tb.time;
    tv->tv_usec = tb.millitm * 1000;
}
