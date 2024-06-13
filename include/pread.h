#ifndef PREAD_LIB_H
#define PREAD_LIB_H

#include <stdint.h>
#include <unistd.h>

#ifdef _WIN32
ssize_t pread(int fd, void *buf, size_t count, uint64_t offset);
#endif

#endif
