/*
 *----------------------------------------------------------------------
 * File: unlink.c
 * Purpose:
 *   Provide a way to unlink files without changing the source code of
 *   tclUnix*.c for Windows NT
 *
 * Notes:
 *   This is not perfect.  It only tries to unlink files when a new unlink
 *   is called.  At this point, is runs through all old files that it could
 *   not previously remove and it removes them.
 *----------------------------------------------------------------------
 */

#include <io.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "tknt_memory.h"

typedef struct _LinkedFile {
    char *fname;
    struct _LinkedFile *next;
} LinkedFile;

LinkedFile *fileList = NULL;

int
compat_unlink(const char *fname)
{
    int ret, error;
    LinkedFile *new, *prev, *curr, *next;

    /* To make this multithread safe, we should use a semaphore to
     * allow continuation here
     */

    /* Run through all names in out list of files to unlink.  Unlink
     * all that we can
     */
    for (prev = NULL, curr = fileList; curr != NULL; curr = next)
    {
	next = curr->next;
	ret = _unlink(curr->fname);
	if (ret == 0) {
	    if (prev) {
		prev->next = curr->next;
	    } else {
		fileList = curr->next;
	    }
	    ckfree(curr);
	} else {
	    prev = curr;
	}
    }

    if (fname == NULL) {
	return 0;
    }

    ret = _unlink(fname);
    if (ret == -1) {
	error = errno;
	if (error == EACCES) {
	    /* Add to the list */
	    new = (LinkedFile *) ckalloc(sizeof(*new));
	    new->fname = ckstrdup(fname);
	    if (new->fname != NULL) {
		new->next = fileList;
		fileList = new;
	    } else {
		ckfree(new);
	    }
	    ret = 0;
	}
    }
    return ret;
}	
