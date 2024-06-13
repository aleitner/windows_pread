#include "pread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

char *usage = "Usage: generate_data <-f file_path> <-s size> [-o offset]\n";

int main(int argc, char **argv) {
    int status = 0;
    uint64_t size = 0;
    uint64_t offset = 0;
    char *file_path = NULL;
    FILE *file = NULL;
    ssize_t read_bytes;

    opterr = 0;

    int c;
    while ((c = getopt(argc, argv, "f:s:o:")) != -1) {
        switch (c) {
            case 'f':
                file_path = optarg;
                break;
            case 's':
                sscanf(optarg, "%" PRIu64, &size);
                break;
            case 'o':
                sscanf(optarg, "%" PRIu64, &offset);
                break;
            case '?':
                if (optopt == 's' || optopt == 'f' || optopt == 'o') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
                fprintf(stderr, "%s", usage);
                return 1;
            default:
                fprintf(stderr, "%s", usage);
                return 1;
        }
    }

    if (file_path == NULL || size == 0) {
        fprintf(stderr, "File path and size are required.\n");
        fprintf(stderr, "%s", usage);
        return 1;
    }

    file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open [%s]\n", file_path);
        return 1;
    }

    fprintf(stderr, "path: %s, size: %" PRIu64 ", offset: %" PRIu64 "\n", file_path, size, offset);

    char *buff = malloc(size + 1);
    if (buff == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(file);
        return 1;
    }
    memset(buff, '\0', size + 1);

    read_bytes = pread(fileno(file), buff, size, offset);
    if (read_bytes == -1) {
        fprintf(stderr, "Error reading file: %s\n", strerror(errno));
        free(buff);
        fclose(file);
        return 1;
    }

    fwrite(buff, 1, read_bytes, stdout);
    fflush(stdout);

    free(buff);
    fclose(file);

    return status;
}
