#include <cstdio>
#include <string>
#include <vector>

#include "pnmforge.hpp"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

static std::string read_stdin() {
    std::string data;
    char buffer[4096];
    for (;;) {
        std::size_t got = std::fread(buffer, 1, sizeof buffer, stdin);
        if (got == 0) {
            break;
        }
        data.append(buffer, got);
    }
    return data;
}

int main(int argc, char **argv) {
    std::vector<std::string> words(argv + 1, argv + argc);
#ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
    _setmode(_fileno(stderr), _O_BINARY);
#endif
    std::string input;
    if (pnmforge::reads_stdin(words)) {
        input = read_stdin();
    }
    pnmforge::Result result = pnmforge::run(words, input);
    std::fwrite(result.out.data(), 1, result.out.size(), stdout);
    std::fwrite(result.err.data(), 1, result.err.size(), stderr);
    std::fflush(stdout);
    return result.status;
}
