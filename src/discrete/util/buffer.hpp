#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

struct buffer
{
    static constexpr size_t BUFFER_DEFAULT_SIZE = 32;
    char* line = nullptr;
    size_t cap = BUFFER_DEFAULT_SIZE;
};

bool
buffer_line(FILE* fp, buffer* b)
{
    if (nullptr == fgets(b->line, b->cap, fp))
        return false;

    // We can assume here that at least one
    // character was read, so strlen(b->line) is at least 1.

    unsigned int offset = 0;

    while (!(feof(fp) || b->line[strlen(b->line) - 1] == '\n')) {
        offset = b->cap;
        b->cap <<= 1;

        void* rptr = realloc(b->line, b->cap);

        if (nullptr == rptr) {
            fprintf(stderr, "Fatal error while enlarging buffer\n");
            return false;
        }

        b->line = (char*)rptr;

        // Now append into "newly allocated half" thereby overwriting
        // the nullptr termination of the former fgets call
        if (nullptr == fgets(b->line + offset - 1, offset + 1, fp))
            return false;
    }

    return true;
}
