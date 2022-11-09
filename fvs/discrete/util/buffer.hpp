#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#ifdef __unix__
#include <unistd.h>
#elif __MSDOS__ || __WIN32__ || _MSC_VER
#include <io.h>
#endif

namespace fvs {
// TODO: move this to a more senseful location
static constexpr std::size_t BUFFER_DEFAULT_SIZE = 32;

bool
buffer_line(FILE* fp, std::vector<char>& b)
{
    if (nullptr == fgets(b.data(), static_cast<int>(b.size()), fp))
        return false;

    // We can assume here that at least one
    // character was read, so strlen(b->line) is at least 1.

    int offset = 0;
    while (!(feof(fp) || b[strlen(b.data()) - 1] == '\n')) {
        offset = static_cast<int>(b.size());
        b.resize(b.size() * 2);

        // Now append into "newly allocated half" thereby overwriting
        // the nullptr termination of the former fgets call
        if (nullptr == fgets(&b[offset - 1], offset + 1, fp))
            return false;
    }

    return true;
}
}