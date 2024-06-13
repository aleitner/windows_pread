#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>

/**
 * Translates a Windows error code to a POSIX errno value.
 *
 * @param dwError
 *     The Windows error code to translate.
 *
 * @return 
 *     The corresponding POSIX errno value.
 */
static int translate_windows_error_to_errno(DWORD dwError) {
    switch (dwError) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            return ENOENT;
        case ERROR_TOO_MANY_OPEN_FILES:
            return EMFILE;
        case ERROR_ACCESS_DENIED:
            return EACCES;
        case ERROR_INVALID_HANDLE:
            return EBADF;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            return ENOMEM;
        case ERROR_NOT_READY:
        case ERROR_CRC:
            return EIO;
        case ERROR_SHARING_VIOLATION:
        case ERROR_LOCK_VIOLATION:
            return EBUSY;
        case ERROR_HANDLE_EOF:
            return 0;
        case ERROR_BROKEN_PIPE:
            return EPIPE;
        default:
            return EINVAL;
    }
}

ssize_t pread(int fd, void *buf, size_t count, uint64_t offset)
{
    long unsigned int read_bytes = 0;

    OVERLAPPED overlapped;
    memset(&overlapped, 0, sizeof(OVERLAPPED));

    overlapped.OffsetHigh = (uint32_t)((offset & 0xFFFFFFFF00000000LL) >> 32);
    overlapped.Offset = (uint32_t)(offset & 0xFFFFFFFFLL);

    HANDLE file = (HANDLE)_get_osfhandle(fd);
    SetLastError(0);
    bool RF = ReadFile(file, buf, count, &read_bytes, &overlapped);

    /* For some reason it errors when it hits end of file so we don't want to check that */
    if ((RF == 0) && GetLastError() != ERROR_HANDLE_EOF) {
        DWORD dwError = GetLastError();
        errno = translate_windows_error_to_errno(dwError);
        return -1;
    }

    return read_bytes;
}
#endif
