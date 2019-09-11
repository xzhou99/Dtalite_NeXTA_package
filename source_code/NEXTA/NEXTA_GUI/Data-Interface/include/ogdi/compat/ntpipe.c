#include <io.h>
#include <fcntl.h>

int
pipe(int *phandles)
{
    return _pipe(phandles, 8192, O_NOINHERIT);
}
