# uerrno
"system error codes"

from typing import Dict

MP_EPERM = 1  # Operation not permitted
MP_ENOENT = 2  # No such file or directory
MP_ESRCH = 3  # No such process
MP_EINTR = 4  # Interrupted system call
MP_EIO = 5  # I/O error
MP_ENXIO = 6  # No such device or address
MP_E2BIG = 7  # Argument list too long
MP_ENOEXEC = 8  # Exec format error
MP_EBADF = 9  # Bad file number
MP_ECHILD = 10  # No child processes
MP_EAGAIN = 11  # Try again
MP_ENOMEM = 12  # Out of memory
MP_EACCES = 13  # Permission denied
MP_EFAULT = 14  # Bad address
MP_ENOTBLK = 15  # Block device required
MP_EBUSY = 16  # Device or resource busy
MP_EEXIST = 17  # File exists
MP_EXDEV = 18  # Cross-device link
MP_ENODEV = 19  # No such device
MP_ENOTDIR = 20  # Not a directory
MP_EISDIR = 21  # Is a directory
MP_EINVAL = 22  # Invalid argument
MP_ENFILE = 23  # File table overflow
MP_EMFILE = 24  # Too many open files
MP_ENOTTY = 25  # Not a typewriter
MP_ETXTBSY = 26  # Text file busy
MP_EFBIG = 27  # File too large
MP_ENOSPC = 28  # No space left on device
MP_ESPIPE = 29  # Illegal seek
MP_EROFS = 30  # Read-only file system
MP_EMLINK = 31  # Too many links
MP_EPIPE = 32  # Broken pipe
MP_EDOM = 33  # Math argument out of domain of func
MP_ERANGE = 34  # Math args not representable
MP_EWOULDBLOCK = MP_EAGAIN  # Operation would block
MP_EOPNOTSUPP = 95  # Operation not supported on transport endpoint
MP_EAFNOSUPPORT = 97  # Address family not supported by protocol
MP_EADDRINUSE = 98  # Address already in use
MP_ECONNABORTED = 103  # Software caused connection abort
MP_ECONNRESET = 104  # Connection reset by peer
MP_ENOBUFS = 105  # No buffer space available
MP_EISCONN = 106  # Transport endpoint is already connected
MP_ENOTCONN = 107  # Transport endpoint is not connected
MP_ETIMEDOUT = 110  # Connection timed out
MP_ECONNREFUSED = 111  # Connection refused
MP_EHOSTUNREACH = 113  # No route to host
MP_EALREADY = 114  # Operation already in progress
MP_EINPROGRESS = 115  # Operation now in progress

errorcode: Dict[int, str] = {}  # Dictionary mapping numeric error codes to strings with symbolic error code
