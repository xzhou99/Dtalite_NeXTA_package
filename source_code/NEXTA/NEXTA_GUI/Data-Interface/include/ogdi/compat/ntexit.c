/*
 * exit.c ---
 *   This replaces exit() with one unlinks any temporary files that
 *   are still lying around.
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void
compat_exit(int status)
{
    extern int compat_unlink();

    compat_unlink(NULL);
    fflush(stdout);
    fflush(stderr);
    ExitProcess(status);
}

