#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"histogram"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "0-31 1\n32-63 1\n64-95 1\n96-127 0\n128-159 1\n160-191 0\n192-223 1\n224-255 1\n", "");
    failed += row({"histogram", "--bins", "4"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "0-63 2\n64-127 1\n128-191 1\n192-255 2\n", "");
    failed += row({"histogram"}, "P1\n3 2\n1 0 1\n0 1 0\n", 0, "0-0 3\n1-1 3\n", "");
    failed += row({"histogram", "--bins", "3"}, "P2 3 3 9\n0 1 2\n3 4 5\n6 7 9\n", 0, "0-2 3\n3-5 3\n6-9 3\n", "");
    failed += row({"histogram", "--bins", "5"}, "P1\n3 2\n1 0 1\n0 1 0\n", 1, "", "pnmforge: too many bins for maxval 1\n");
    failed += row({"histogram", "--bins", "2"}, "P2 2 2 1\n0 1\n1 1\n", 0, "0-0 1\n1-1 3\n", "");
    failed += row({"histogram"}, "P3\n2 1\n255\n255 0 0 0 255 0\n", 1, "", "pnmforge: histogram needs a P1 or P2 image\n");
    return failed != 0;
}
