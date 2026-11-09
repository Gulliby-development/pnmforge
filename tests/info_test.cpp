#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"info"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "format P2\nsize 3x2\nmaxval 255\nsamples 6\nmin 0\nmax 255\nsum 671\n", "");
    failed += row({"info", "--brief"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "P2 3x2 255\n", "");
    failed += row({"info"}, "P1\n3 2\n1 0 1\n0 1 0\n", 0, "format P1\nsize 3x2\nmaxval 1\nsamples 6\nmin 0\nmax 1\nsum 3\n", "");
    failed += row({"info", "--brief"}, "P1\n3 2\n1 0 1\n0 1 0\n", 0, "P1 3x2 1\n", "");
    failed += row({"info"}, "P3\n2 1\n255\n255 0 0 0 255 0\n", 0, "format P3\nsize 2x1\nmaxval 255\nsamples 6\nmin 0\nmax 255\nsum 510\n", "");
    failed += row({"info"}, "P2 1 1 255 0", 0, "format P2\nsize 1x1\nmaxval 255\nsamples 1\nmin 0\nmax 0\nsum 0\n", "");
    failed += row({"info"}, "P2 8 4 255\n255 255 255 255 255 255 255 255\n255 255 255 255 255 255 255 255\n255 255 255 255 255 255 255 255\n255 255 255 255 255 255 255 255\n", 0, "format P2\nsize 8x4\nmaxval 255\nsamples 32\nmin 255\nmax 255\nsum 8160\n", "");
    return failed != 0;
}
