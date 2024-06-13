#include "pread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    off_t offset;
    size_t size;
    const char* expected;
    int expected_errno;
} TestCase;

int run_test_case(int fd, const TestCase* testCase) {
    char buffer[testCase->size + 1];
    memset(buffer, 0, testCase->size + 1);

    ssize_t bytes_read = pread(fd, buffer, testCase->size, testCase->offset);
    if ((bytes_read < 0 && errno != testCase->expected_errno) || 
        (bytes_read >= 0 && testCase->expected_errno != 0)) {
        fprintf(stderr, "Test failed: expected errno %d, got %d\n", testCase->expected_errno, errno);
        return -1;
    }

    if (bytes_read >= 0 && strcmp(buffer, testCase->expected) != 0) {
        fprintf(stderr, "Test failed: expected '%s', got '%s'\n", testCase->expected, buffer);
        return -1;
    }

    if (bytes_read >= 0)
        printf("Test passed: read '%s' from offset %ld\n", buffer, (long)testCase->offset);

    else
        printf("Test passed: errno %d as expected\n", testCase->expected_errno);

    return 0;
}

int create_and_unlink_temp_file(const char* content) {
    int fd = -1;

#ifdef _WIN32
    char tempPath[MAX_PATH];
    char tempFileName[MAX_PATH];

    /* Retrieve the temporary files directory*/
    DWORD pathLen = GetTempPath(MAX_PATH, tempPath);
    if (pathLen == 0 || pathLen > MAX_PATH) {
        perror("Failed to get temp path");
        return -1;
    }

    /* Create a temporary file name*/
    UINT uniqueNum = GetTempFileName(tempPath, "TMP", 0, tempFileName);
    if (uniqueNum == 0) {
        perror("Failed to create temp file name");
        return -1;
    }

    /* Open the temporary file*/
    fd = open(tempFileName, O_RDWR | O_CREAT | O_EXCL, _S_IREAD | _S_IWRITE);
    if (fd == -1) {
        perror("Failed to open temp file");
        return -1;
    }

    /* Mark the file for deletion when it's closed*/
    if (!DeleteFile(tempFileName)) {
        perror("Failed to mark temp file for deletion");
        close(fd);
        return -1;
    }

#else
    char template[] = "/tmp/tempfileXXXXXX";
    fd = mkstemp(template);
    if (fd == -1) {
        perror("Failed to create temporary file");
        return -1;
    }
    
    /* Unlink the file immediately so that it's removed upon closing */
    unlink(template);
#endif

    /* Write the specified content to the temporary file */
    if (write(fd, content, strlen(content)) != strlen(content)) {
        perror("Failed to write to temporary file");
        close(fd);
        return -1;
    }

    /* Reset file position to the beginning for reading */
    lseek(fd, 0, SEEK_SET);

    return fd;
}

int main() {
    const char* content = "Hello World";
    
    int fd = create_and_unlink_temp_file(content);
    if (fd < 0) {
        return -1;
    }

    TestCase testCases[] = {
        {0, 5, "Hello", 0},
        {6, 5, "World", 0},
        {0, 50, "Hello World", 0},
        {-1, 0, "", EINVAL}
    };
    int numTestCases = sizeof(testCases) / sizeof(testCases[0]);

    for (int i = 0; i < numTestCases; ++i) {
        if (run_test_case(fd, &testCases[i]) < 0) {
            close(fd);
            return -1;
        }
    }

    close(fd);
    return 0;
}
