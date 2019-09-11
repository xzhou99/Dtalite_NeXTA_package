#ifdef _WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
#endif /* ifdef _WINDOWS */
#include <tcl.h>
#include <stdlib.h>
#include <io.h>

/*
 *----------------------------------------------------------------------
 *
 * try_create --
 *
 *	Try to find a valid temporary filename that can be written to.
 *
 * Notes:
 *	XXX: This uses an incredibly stupid algorithm to decide failures.
 *	It needs to be changed to find out if we don't have permission
 *	to write in the directory.
 *
 * Results:
 *	TRUE if successful, FALSE if not.
 *
 * Side Effects:
 *	The filename is set in name
 *
 *----------------------------------------------------------------------
 */
static int
try_create(char *name, char *dir, char *prefix)
{
    HANDLE file;
    int first, len, plen;

    static int last_prefix = 0;

    if (_access(dir, 06) != 0) {
	return FALSE;
    }
    len = strlen(dir);
    memcpy(name, dir, len);
    if (len && (dir[len-1] != '/' && dir[len-1] != '\\')) {
	name[len] = '\\';
	len++;
    }
    plen = strlen(prefix);
    memcpy(&name[len], prefix, plen);
    len += plen;
	
    for (first = last_prefix, last_prefix++; last_prefix != first;
	 last_prefix %= 65536) {
	sprintf(&name[len], "%d", last_prefix);
	file = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			  FILE_FLAG_DELETE_ON_CLOSE, NULL);
	if (file != INVALID_HANDLE_VALUE) {
	    CloseHandle(file);
	    return TRUE;
	}
    }
    return FALSE;
}

char *
compat_tempnam(char *dir, char *prefix)
{
    char *tmpdir;
    int len, maxlen;
    char *name;

    maxlen = strlen(dir);
    tmpdir = getenv("TMP");
    if (tmpdir != NULL) {
	len = strlen(tmpdir);
	if (len > maxlen) {
	    maxlen = len;
	}
    }
    len = strlen(_P_tmpdir);
    if (len > maxlen) {
	maxlen = len;
    }

    maxlen += strlen(prefix) + 10;
    name = ckalloc(maxlen);
    if (tmpdir && try_create(name, tmpdir, prefix)) {
	return name;
    }
    if (try_create(name, dir, prefix)) {
	return name;
    }

    if (try_create(name, _P_tmpdir, prefix)) {
	return name;
    }
    ckfree(name);
    return NULL;
}
