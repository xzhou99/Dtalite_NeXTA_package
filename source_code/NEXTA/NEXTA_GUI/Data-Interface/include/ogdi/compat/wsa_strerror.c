#include <windows.h>
#include <winsock.h>

char *
wsa_strerror(error)
    int error;
{
    switch (error) {
    /*
     * Windows Sockets definitions of regular Microsoft C error constants
     */
    case WSAEINTR:
	return "Winsock call interrupted.";
    case WSAEBADF:
	return "Bad file in call to Winsock.";
    case WSAEACCES:
	return "Bad access in call to Winsock.";
    case WSAEFAULT:
	return "Bad argument in call to Winsock.";
    case WSAEINVAL:
	return "Invalid call to Winsock.";
    case WSAEMFILE:
	return "Out of file descriptors in call to Winsock.";

    /*
     * Windows Sockets definitions of regular Berkeley error constants
     */
    case WSAEWOULDBLOCK:
	return "Winsock call would block.";
    case WSAEINPROGRESS:
	return "Blocking call to Winsock in progress.";
    case WSAEALREADY:
	return "Winsock call already?";
    case WSAENOTSOCK:
	return "Descriptor is not a socket.";
    case WSAEDESTADDRREQ:
	return "Need destination address in call to Winsock.";
    case WSAEMSGSIZE:
	return "Bad message size in call to Winsock.";
    case WSAEPROTOTYPE:
	return "Bad protocol in call to Winsock.";
    case WSAENOPROTOOPT:
	return "Protocol option is unsupported in call to Winsock.";
    case WSAEPROTONOSUPPORT:
	return "Protocol is unsupported in call to Winsock.";
    case WSAESOCKTNOSUPPORT:
	return "Socket is unsupported by Winsock.";
    case WSAEOPNOTSUPP:
	return "Operation not supported in call to Winsock.";
    case WSAEPFNOSUPPORT:
	return "Address family not supported this protocol in Winsock.";
    case WSAEADDRINUSE:
	return "Address already in use in call to Winsock.";
    case WSAEADDRNOTAVAIL:
        return "Address not available in call to Winsock.";
    case WSAENETDOWN:
	return "Network subsystem has failed in call to Winsock.";
    case WSAENETUNREACH:
	return "Network is unreachable in call to Winsock.";
    case WSAENETRESET:
	return "Network has been reset in call to Winsock.";
    case WSAECONNABORTED:
	return "Connection was apported in call to Winsock.";
    case WSAECONNRESET:
	return "Connection was reset in call to Winsock.";
    case WSAENOBUFS:
	return "No buffer space is available in call to Winsock.";
    case WSAEISCONN:
	return "Socket is already connected in call to Winsock.";
    case WSAENOTCONN:
	return "Socket is not connected in call to Winsock.";
    case WSAESHUTDOWN:
	return "Socket has been shut down in call to Winsock.";
    case WSAETOOMANYREFS:
	return "Too many references in call to Winsock.";
    case WSAETIMEDOUT:
	return "Timed out in call to Winsock.";
    case WSAECONNREFUSED:
	return "Connection refused";
    case WSAELOOP:
	return "Loop in call to Winsock.";
    case WSAENAMETOOLONG:
	return "Name too long in call to Winsock.";
    case WSAEHOSTDOWN:
	return "Host is down in call to Winsock.";
    case WSAEHOSTUNREACH:
	return "Host is unreachable in call to Winsock.";
    case WSAENOTEMPTY:
	return "Not empty in call to Winsock.";
    case WSAEPROCLIM:
	return "Proclim? in call to Winsock.";
    case WSAEUSERS:
	return "Users? in call to Winsock.";
    case WSAEDQUOT:
	return "Bad quote? in call to Winsock.";
    case WSAESTALE:
	return "Something is stale in call to Winsock.";
    case WSAEREMOTE:
	return "Remote error in call to Winsock.";
    case WSAEDISCON:
	return "Disconnected in call to Winsock.";
	
    /*
     * Extended Windows Sockets error constant definitions
     */
    case WSASYSNOTREADY:
	return "Winsock library is not yet ready for use.";
    case WSAVERNOTSUPPORTED:
	return "Winsock version not supported.";
    case WSANOTINITIALISED:
	return "Winsock library has not been initalized before use.";

    /*
     * Error return codes from gethostbyname() and gethostbyaddr()
     * (when using the resolver). Note that these errors are
     * retrieved via WSAGetLastError() and must therefore follow
     * the rules for avoiding clashes with error numbers from
     * specific implementations or language run-time systems.
     * For this reason the codes are based at WSABASEERR+1001.
     * Note also that [WSA]NO_ADDRESS is defined only for
     * compatibility purposes.
     */

    /* Authoritative Answer: Host not found */
    case WSAHOST_NOT_FOUND:
	return "Host not found.";

    /* Non-Authoritative: Host not found, or SERVERFAIL */
    case WSATRY_AGAIN:
	return "Host not found, try again.";

    /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
    case WSANO_RECOVERY:
	return "Unrecoverable error in call to nameserver.";

    /* Valid name, no data record of requested type */
    case WSANO_DATA:
	return "No data record of requested type.";

    default:
	return strerror(error);
    }
}

