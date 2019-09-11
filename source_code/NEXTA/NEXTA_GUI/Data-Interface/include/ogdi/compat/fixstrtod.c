/* 
 * fixstrtod.c --
 *
 *	Source code for the "fixstrtod" procedure.  This procedure is
 *	used in place of strtod under Solaris 2.4, in order to fix
 *	a bug where the "end" pointer gets set incorrectly.
 *
 * Copyright (c) 1995 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

static char sccsid[] = "@(#) fixstrtod.c 1.1 95/05/06 17:40:33";

#undef strtod
#include <stdlib.h>

double
fixstrtod(string, endPtr)
    char *string;
    char **endPtr;
{
    double d;
    d = strtod(string, endPtr);
    if ((*endPtr != string) && ((*endPtr)[-1] == 0)) {
	*endPtr -= 1;
    }
    return d;
}
