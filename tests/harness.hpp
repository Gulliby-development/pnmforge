#ifndef PNM_HARNESS_HPP
#define PNM_HARNESS_HPP

#include <cstdio>
#include <string>
#include <vector>

#include "pnmforge.hpp"

/* row_at runs the command line with the text as standard input and compares the exit status, the standard output and
   the standard error byte for byte; it returns 0 when they are equal and 1 otherwise. */
inline int row_at(const char *file, int line, const std::vector<std::string> &args, const std::string &input, int status,
                  const std::string &out, const std::string &err) {
    pnmforge::Result result = pnmforge::run(args, input);
    bool ok = result.status == status && result.out == out && result.err == err;
    if (!ok) {
        std::fprintf(stderr, "%s:%d: row failed: status %d, stdout \"%s\", stderr \"%s\"\n", file, line, result.status,
                     result.out.c_str(), result.err.c_str());
    }
    return ok ? 0 : 1;
}

/* reads_at compares pnmforge::reads_stdin with the expected value (true or false). */
inline int reads_at(const char *file, int line, const std::vector<std::string> &args, bool expected) {
    bool got = pnmforge::reads_stdin(args);
    if (got != expected) {
        std::fprintf(stderr, "%s:%d: reads failed: got %d, expected %d\n", file, line, got ? 1 : 0, expected ? 1 : 0);
    }
    return got == expected ? 0 : 1;
}

#define row(...) row_at(__FILE__, __LINE__, __VA_ARGS__)
#define reads(...) reads_at(__FILE__, __LINE__, __VA_ARGS__)

#endif
