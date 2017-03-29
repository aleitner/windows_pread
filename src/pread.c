#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>

char *usage = "Usage: generate_data <-f file_path> <-s size> [-o offset]\n";

#ifdef _WIN32
#include <windows.h>
#include <io.h>

ssize_t pread(int fd, void *buf, size_t count, uint64_t offset);
#endif

int main (int argc, char **argv)
{
    int status = 0;
    int index;
    int c;
    uint64_t size = 0;
    uint64_t offset = 0;
    char *file_path = NULL;
    FILE *file = NULL;
    uint64_t read_bytes;

    opterr = 0;

    while ((c = getopt (argc, argv, "f:s:o:")) != -1) {
        switch (c) {
            case 'f':
                file_path = optarg;
                file = fopen(file_path, "r");
                if (file == NULL) {
                    fprintf(stderr, "Could not open [%s]\n", file_path);
                    return 1;
                }
                break;
            case 's':
                sscanf(optarg, "%"PRIu64, &size);
                break;
            case 'o':
                sscanf(optarg, "%"PRIu64, &offset);
                break;
            case '?':
                if (optopt == 's') {
                    fprintf(stderr, "Option -%c requires a size to be read in bytes.\n", optopt);
                } else if (optopt == 'f') {
                    fprintf(stderr, "Option -%c requires a file path.\n", optopt);
                } else if (optopt == 'o') {
                    fprintf(stderr, "Option -%c requires an offset.\n", optopt);
                }

                return 1;
            default:
                return 1;
        }
    }

    fprintf(stderr, "path: %s, size: %"PRIu64", offset: %"PRIu64"\n", file_path, size, offset);

    char buff[size +1];
    memset(buff, '\0', size + 1);
    read_bytes = pread(fileno(file), buff, size, offset);

    if (read_bytes == -1) {
        fprintf(stderr, "Error reading file: %d\n", errno);
        return 1;
    }

    fprintf(stdout, buff);

    if(file){
        fclose(file);
    }

    return status;
}

#ifdef _WIN32
ssize_t pread(int fd, void *buf, size_t count, uint64_t offset)
{
    long unsigned int read_bytes = 0;

    OVERLAPPED overlapped;
    memset(&overlapped, 0, sizeof(OVERLAPPED));

    overlapped.Offset = offset;

    HANDLE file = (HANDLE)_get_osfhandle(fd);
    SetLastError(0);
    bool RF = ReadFile(file, buf, count, &read_bytes, &overlapped);

     // For some reason it errors when it hits end of file so we don't want to check that
    if ((RF == 0) && GetLastError() != ERROR_HANDLE_EOF) {
        errno = GetLastError();
        // printf ("Error reading file : %d\n", GetLastError());
        return -1;
    }

    return read_bytes;
}
#endif
