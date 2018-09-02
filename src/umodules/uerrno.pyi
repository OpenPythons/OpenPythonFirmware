"""
/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
"""

MP_EPERM: int  # Operation not permitted
MP_ENOENT: int  # No such file or directory
MP_ESRCH: int  # No such process
MP_EINTR: int  # Interrupted system call
MP_EIO: int  # I/O error
MP_ENXIO: int  # No such device or address
MP_E2BIG: int  # Argument list too long
MP_ENOEXEC: int  # Exec format error
MP_EBADF: int  # Bad file number
MP_ECHILD: int  # No child processes
MP_EAGAIN: int  # Try again
MP_ENOMEM: int  # Out of memory
MP_EACCES: int  # Permission denied
MP_EFAULT: int  # Bad address
MP_ENOTBLK: int  # Block device required
MP_EBUSY: int  # Device or resource busy
MP_EEXIST: int  # File exists
MP_EXDEV: int  # Cross-device link
MP_ENODEV: int  # No such device
MP_ENOTDIR: int  # Not a directory
MP_EISDIR: int  # Is a directory
MP_EINVAL: int  # Invalid argument
MP_ENFILE: int  # File table overflow
MP_EMFILE: int  # Too many open files
MP_ENOTTY: int  # Not a typewriter
MP_ETXTBSY: int  # Text file busy
MP_EFBIG: int  # File too large
MP_ENOSPC: int  # No space left on device
MP_ESPIPE: int  # Illegal seek
MP_EROFS: int  # Read-only file system
MP_EMLINK: int  # Too many links
MP_EPIPE: int  # Broken pipe
MP_EDOM: int  # Math argument out of domain of func
MP_ERANGE: int  # Math args not representable
MP_EWOULDBLOCK: int  # Operation would block
MP_EOPNOTSUPP: int  # Operation not supported on transport endpoint
MP_EAFNOSUPPORT: int  # Address family not supported by protocol
MP_EADDRINUSE: int  # Address already in use
MP_ECONNABORTED: int  # Software caused connection abort
MP_ECONNRESET: int  # Connection reset by peer
MP_ENOBUFS: int  # No buffer space available
MP_EISCONN: int  # Transport endpoint is already connected
MP_ENOTCONN: int  # Transport endpoint is not connected
MP_ETIMEDOUT: int  # Connection timed out
MP_ECONNREFUSED: int  # Connection refused
MP_EHOSTUNREACH: int  # No route to host
MP_EALREADY: int  # Operation already in progress
MP_EINPROGRESS: int  # Operation now in progress
