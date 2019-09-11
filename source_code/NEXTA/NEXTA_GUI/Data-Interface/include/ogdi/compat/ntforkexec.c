#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <tcl.h>

static char *FindExec(char *);
static int Directory(char *);

static char *argv0_dir;

static int WindowsNT = -1;

/*
 *----------------------------------------------------------------------
 * FindExec --
 *
 *	Find <command> by going through the directories in the 'PATH' or
 *	'Path' environment variable
 *
 * Results:
 *	A fully qualified path to the executable.
 *----------------------------------------------------------------------
 */
static char *
FindExec(char *command)
{
    char *path = NULL;
    char *newpath, *bp, *start, *dir;
    char save;
    int len, need, cmdlen, dirlen;
    static int have = 0;
    static char *exec = NULL;

    /*
     * Check for the case where the command name has some path. In
     * this case we just check for possible extensions.
     */
    len = strlen(command);
    if (len > 2) {
	if (command[1] == ':' ||
	    strchr(command, '/') != NULL || strchr(command, '\\') != NULL) {
	    cmdlen = strlen(command);
	    need = cmdlen + 5;
	    if (need > have) {
		if (exec) {
		    ckfree(exec);
		}
		exec = (char *) ckalloc(need);
		if (exec == NULL) {
		    return command;
		}
		have = need;
	    }
	    strcpy(exec, command);
	    strcpy(&exec[cmdlen], ".exe");
	    if (access(exec, 0) == 0) {
		if (! Directory(exec)) {
		    goto fixSlashes;
		}
	    }
	    strcpy(&exec[cmdlen], ".bat");
	    if (access(exec, 0) == 0) {
		if (! Directory(exec)) {
		    goto fixSlashes;
		}
	    }
	    if (WindowsNT == 1) {
		strcpy(&exec[cmdlen], ".cmd");
		if (access(exec, 0) == 0) {
		    if (! Directory(exec)) {
			goto fixSlashes;
		    }
		}
	    }
	    strcpy(&exec[cmdlen], ".com");
	    if (access(exec, 0) == 0) {
		if (! Directory(exec)) {
		    goto fixSlashes;
		}
	    }
	    strcpy(&exec[cmdlen], ".tcl");
	    if (access(exec, 0) == 0) {
		if (! Directory(exec)) {
		    goto fixSlashes;
		}
	    }
	    return command;
	}
    }

    path = getenv("PATH");
    if (path == NULL) {
	path = getenv("Path");
	if (path == NULL) {
	    return command;
	}
    }

    len = (argv0_dir) ? strlen(argv0_dir) : 0;
    newpath = ckalloc(len + strlen(path) + 2);
    if (newpath == NULL) {
	ckfree(path);
	return command;
    }

    if (len > 0) {
	strcpy(newpath, argv0_dir);
	newpath[len-1] = ';';
    }
    strcpy(&newpath[len], path);
    path = newpath;
    
    /*
     * the command has no path part so go looking through all the paths
     * in the "Path" or "PATH" environment variable.
     */
    for (start = path, bp = start; *start != '\0'; start = bp) {
	while(*bp != '\0' && *bp != ';') {
	    bp++;
	}
	save = *bp;
	*bp = '\0';
	dirlen = strlen(start);
	if (dirlen == 0) {
	    dir = "./";
	    dirlen = 2;
	} else {
	    dir = start;
	}
	cmdlen = strlen(command);
	need = cmdlen + dirlen + 6;
	if (need > have) {
	    if (exec) {
		ckfree(exec);
	    }
	    exec = (char *) ckalloc(need);
	    if (exec == NULL) {
		ckfree(path);
		return command;
	    }
	    have = need;
	}
	strcpy(exec, dir);
	if (dir[dirlen-1] != '/' && dir[dirlen-1] != '\\') {
	    exec[dirlen] = '\\';
	    dirlen++;
	}
	strcpy(&exec[dirlen], command);
	if (access(exec, 0) == 0) {
	    if (! Directory(exec)) {
		goto fixSlashes;
	    }
	}
	len = dirlen + cmdlen;
	strcpy(&exec[len], ".exe");
	if (access(exec, 0) == 0) {
	    if (! Directory(exec)) {
		goto fixSlashes;
	    }
	}
	strcpy(&exec[len], ".bat");
	if (access(exec, 0) == 0) {
	    if (! Directory(exec)) {
		goto fixSlashes;
	    }
	}
	if (WindowsNT == 1) {
	    strcpy(&exec[len], ".cmd");
	    if (access(exec, 0) == 0) {
		if (! Directory(exec)) {
		    goto fixSlashes;
		}
	    }
	}
	strcpy(&exec[len], ".com");
	if (access(exec, 0) == 0) {
	    if (! Directory(exec)) {
		goto fixSlashes;
	    }
	}
	strcpy(&exec[len], ".tcl");
	if (access(exec, 0) == 0) {
	    if (! Directory(exec)) {
		goto fixSlashes;
	    }
	}
	*bp = save;
	bp++;
    }
    ckfree(path);
    return command;

  fixSlashes:
    if (path) {
	ckfree(path);
    }
    for (bp = exec; *bp != '\0' && *bp != ' '; bp++) {
	if (*bp == '/') {
	    *bp = '\\';
	}
    }
    return exec;
}

/*
 *----------------------------------------------------------------------
 * forkexec --
 * 
 *	Execute a program. The program name is <execName> and the
 *	arguments are in <argv>.   <fdstdin>, <fdstdout>, and <fdstderr>
 *	are the file ids of the files to use for stdin,  stdout, and
 *	stderr. <joinThisError> is 1 if stderr should be piped through
 *	<fdstdout>.
 *
 *	This better process handling code came from the ts package by
 *	Grant Munsey <grant@cognicon.com>
 *	
 *----------------------------------------------------------------------
 */
int
forkexec(char *execName, char **argv,
	 int fdstdin, int fdstdout, int fdstderr,
	 int joinThisError)
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    DWORD err;
    char *argstr;
    int rslt = -1;
    HANDLE hin = INVALID_HANDLE_VALUE;
    HANDLE hout = INVALID_HANDLE_VALUE;
    HANDLE herr = INVALID_HANDLE_VALUE;
    HANDLE savestdout, savestdin, savestderr;
    int newfdstdin = -1, newfdstdout = -1, newfdstderr = -1;
    extern int errno;
    char *ep, *sp;
    int hasconsole;
    Tcl_DString dString;
    char **s;
    
    hasconsole = 1;
    
    if (WindowsNT == -1) {
	OSVERSIONINFO version;
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&version)) {
	    WindowsNT = (version.dwPlatformId == VER_PLATFORM_WIN32_NT) ? 1 : 0;
	} else {
	    WindowsNT = 0;
	}
    }

    /*
     * use DuplicateHandle to get inheritable handles for each of the files
     * that is not already ok
     */
    if (fdstdin != -1) {
	if (fdstdin > 2) {
	    newfdstdin = dup(fdstdin);
	} else {
	    newfdstdin = fdstdin;
	}
	hin = (HANDLE)_get_osfhandle(newfdstdin);
	savestdin = GetStdHandle(STD_INPUT_HANDLE);
	SetStdHandle(STD_INPUT_HANDLE, hin);
    }

    if (fdstdout != -1) {
	if (fdstdout > 2) {
	    newfdstdout = dup(fdstdout);
	} else {
	    newfdstdout = fdstdout;
	}
	hout = (HANDLE)_get_osfhandle(newfdstdout);
	savestdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetStdHandle(STD_OUTPUT_HANDLE, hout);
    }
    
    if (joinThisError) {
	herr = (HANDLE)_get_osfhandle(newfdstdout);
	savestderr = GetStdHandle(STD_ERROR_HANDLE);
	SetStdHandle(STD_ERROR_HANDLE, herr);
    }
    
    if (!joinThisError && fdstderr != -1) {
	if (fdstderr > 2) {
	    newfdstderr = dup(fdstderr);
	} else {
	    newfdstderr = fdstderr;
	}
	herr = (HANDLE)_get_osfhandle(newfdstderr);
	savestderr = GetStdHandle(STD_ERROR_HANDLE);
	SetStdHandle(STD_ERROR_HANDLE, herr);
    }
    
    memset((void *) &si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    
    /*
     * get the actual fully qualified path name to the executable
     */
    ep = FindExec(execName);

    /*
     * If the executable is '.bat' or '.cmd' file then we must make sure
     * the path seperator character in the passed argument string for the
     * first parameter is '\'
     */

    Tcl_DStringInit(&dString);
    Tcl_DStringAppend(&dString, execName, -1);
    for (s = &argv[1]; *s != NULL; s++) {
	Tcl_DStringAppend(&dString, " ", 1);
	Tcl_DStringAppend(&dString, *s, -1);
    }

    argstr = Tcl_DStringValue(&dString);

    /*
     * Change all occurrences of '/' to '\\' in program file name
     * and command name since there seem to be some programs that
     * aren't happy any other way.
     */
    for (sp = argstr; *sp != '\0' && *sp != ' '; sp++) {
	if (*sp == '/') {
	    *sp = '\\';
	}
    }

    errno = 0;
    if (!CreateProcess(ep, argstr, NULL, NULL, TRUE /* bInheritHandles */,
		       (hasconsole ? CREATE_NEW_PROCESS_GROUP:
			DETACHED_PROCESS),
		       NULL, NULL, &si, &pi))
    {
	err = GetLastError();
	if (err == ERROR_ACCESS_DENIED) {
	    errno = EACCES;
	} else if (err == ERROR_BAD_EXE_FORMAT) {
	    errno = ENOEXEC;
	} else {
	    errno = ENOENT;
	}
	Tcl_DStringFree(&dString);
	Tcl_DStringInit(&dString);
	if (WindowsNT) {
	    Tcl_DStringAppend(&dString, "cmd.exe /c ", 10);
	} else {
	    Tcl_DStringAppend(&dString, "command.com /c ", 14);
	}
	Tcl_DStringAppend(&dString, " ", 1);
	Tcl_DStringAppend(&dString, ep, -1);

	for (s = &argv[1]; *s != NULL; s++) {
	    Tcl_DStringAppend(&dString, " ", 1);
	    Tcl_DStringAppend(&dString, *s, -1);
	}
	if (WindowsNT) {
	    ep = FindExec("cmd.exe");
	} else {
	    ep = FindExec("command.com");
	}
	if (CreateProcess(ep, argstr, NULL, NULL, TRUE /* bInheritHandles */,
			  (hasconsole ? CREATE_NEW_PROCESS_GROUP:
			   DETACHED_PROCESS),
			  NULL, NULL, &si, &pi))
	{
	    errno = 0;
	}
    }
    if (errno == 0) {
	extern void addpid(DWORD, HANDLE); /* Defined in ntwaitpid.c */
	rslt = (int)pi.dwProcessId;
	addpid(pi.dwProcessId, pi.hProcess);
	CloseHandle(pi.hThread);
    }
    Tcl_DStringFree(&dString);

    if (hin != INVALID_HANDLE_VALUE) {
	SetStdHandle(STD_INPUT_HANDLE, savestdin);
	if (newfdstdin != fdstdin) {
	    close(newfdstdin);
	}
    }
    if (hout != INVALID_HANDLE_VALUE) {
	SetStdHandle(STD_OUTPUT_HANDLE, savestdout);
	if (newfdstdout != fdstdout) {
	    close(newfdstdout);
	}
    }
    if (herr != INVALID_HANDLE_VALUE) {
	SetStdHandle(STD_ERROR_HANDLE, savestderr);
	if (newfdstderr != fdstderr) {
	    close(newfdstderr);
	}
    }
	
    return rslt;
}

void
forkexec_setargv0_dir(char *largv0_dir)
{
    argv0_dir = largv0_dir;
}

int
Directory(char *path)
{
    struct stat statBuf;

    if (stat(path, &statBuf) == -1) {
	return 0;
    }
    if (_S_IFDIR & statBuf.st_mode) {
	return 1;
    }
    return 0;
}
