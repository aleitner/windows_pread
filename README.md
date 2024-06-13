# windows_pread

`windows_pread` is a library designed to provide POSIX `pread` functionality on Windows platforms. This library enables developers to use `pread`-like operations in their Windows applications, facilitating easier code portability between UNIX/Linux and Windows systems.

## Getting Started

### Prerequisites

- A C/C++ compiler compatible with Windows, such as GCC through MinGW or Microsoft Visual C++.
- GNU Make for building the project.

### Building from Source

1. Clone the repository to your local machine:

```bash
git clone https://github.com/yourusername/windows_pread.git
cd windows_pread
```

2. Build the library:

```bash
make libraries
```

This command compiles the `windows_pread` library and generates both static and shared library files in the `lib/` directory.

### Installation

After building, include the static (`libpread.a`) or shared (`libpread.so`) library in your project's library path. Also, ensure the `pread.h` header file is accessible in your project's include path.

### Usage

To use `windows_pread` in your application, include the `pread.h` header file and link against the library:

```c
#include "pread.h"

int fd = open("example_file.txt", O_RDONLY);
if (fd == -1) {
    perror("open");
    return 1;
}

char buf[1024];
ssize_t bytes_read = pread(fd, buf, sizeof(buf), 50);
if (bytes_read == -1) {
    perror("pread");
    close(fd);
    return 1;
}

close(fd);
```

When compiling your application, link it against `libpread.a` for static linking or `libpread.so` for dynamic linking.

### Testing

To verify the functionality of `windows_pread`, you can run the included tests by executing the following command in the project root:

```bash
make check
```

### Contributing

We welcome contributions to `windows_pread`! If you have suggestions for improvements or encounter any issues, please feel free to submit pull requests or open issues on GitHub.

### License

`windows_pread` is provided under the Apache License 2.0. See the LICENSE file for more details.
