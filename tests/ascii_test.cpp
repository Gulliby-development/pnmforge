#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"ascii"}, "P2 5 1 255\n0 64 128 192 255\n", 0, "@#=: \n", "");
    failed += row({"ascii"}, "P1\n3 2\n1 0 1\n0 1 0\n", 0, " @ \n@ @\n", "");
    failed += row({"ascii"}, "P3 2 1 255\n255 0 0 0 255 0\n", 0, "*=\n", "");
    failed += row({"ascii", "--ramp", "01"}, "P2 4 1 9\n0 4 5 9\n", 0, "0011\n", "");
    failed += row({"ascii", "--ramp", ".oX"}, "P2 3 1 4\n0 1 4\n", 0, ".oX\n", "");
    failed += row({"ascii", "--ramp", "ab c"}, "P2 2 2 3\n0 1\n2 3\n", 0, "ab\n c\n", "");
    return failed != 0;
}
