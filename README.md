build on osx
OSX
`gcc src/pread.c -o bin/pread_darwin`

Windows
32 bit
`/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-gcc src/pread.c -o bin/pread_win32.exe`

Linux
32 bit
`/usr/local/gcc-4.8.1-for-linux32/bin/i586-pc-linux-gcc --static src/pread.c -o bin/pread_linux32`

64 bit
`/usr/local/gcc-4.8.1-for-linux64/bin/x86_64-pc-linux-gcc --static src/pread.c -o bin/pread_linux64`

run
`bin/pread -f file_path -s size -o offset`
