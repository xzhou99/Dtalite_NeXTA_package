#include <windows.h>
#include <process.h>
#include <sys/wait.h>
#include <io.h>
#include "tknt_memory.h"

typedef struct pid_node {
    pid_t pid;
    HANDLE process;
    struct pid_node *next;
} pid_node;

static pid_node *pidList = NULL;

void
addpid(DWORD pid, HANDLE process)
{
    pid_node *new;

    new = (pid_node *) ckalloc(sizeof(pid_node));
    new->pid = (pid_t) pid;
    new->process = process;
    new->next = pidList;
    pidList = new;
}

HANDLE
tclnt_getprocesshandle(DWORD pid)
{
    pid_node *p;

    for (p = pidList; p; p = p->next) {
	if (p->pid == (pid_t) pid) {
	    return p->process;
	}
    }
    return NULL;
}

pid_t
waitpid(pid_t pid, int *stat_loc, int options)
{
    DWORD timeout;
    HANDLE process;
    pid_node *prev, *curr;

    for (prev = NULL, curr = pidList; curr != NULL;
	 prev = curr, curr = curr->next)
    {
	if (curr->pid == pid) {
	    break;
	}
    }
    if (curr == NULL) {
	return 0;
    }

    if (options == WNOHANG) {
	timeout = 0;
    } else {
	timeout = INFINITE;
    }
    process = curr->process;
    if (WaitForSingleObject(process, timeout) == WAIT_OBJECT_0) {
	pid = _cwait(stat_loc, (pid_t) process, 0);
	CloseHandle(process);
	if (prev) {
	    prev->next = curr->next;
	} else {
	    pidList = curr->next;
	}
	ckfree((char *) prev);
	return pid;
    }
    return 0;
}

