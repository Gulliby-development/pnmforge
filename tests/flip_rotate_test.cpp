#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"flip", "h"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "P2\n3 2\n255\n255 128 0\n32 192 64\n", "");
    failed += row({"flip", "v"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "P2\n3 2\n255\n64 192 32\n0 128 255\n", "");
    failed += row({"flip", "h"}, "P3\n2 1\n255\n255 0 0 0 255 0\n", 0, "P3\n2 1\n255\n0 255 0 255 0 0\n", "");
    failed += row({"flip", "x"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 2, "", "pnmforge: flip needs h or v\n");
    failed += row({"rotate", "90"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 0, "P2\n3 2\n9\n5 3 1\n6 4 2\n", "");
    failed += row({"rotate", "180"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 0, "P2\n2 3\n9\n6 5\n4 3\n2 1\n", "");
    failed += row({"rotate", "270"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 0, "P2\n3 2\n9\n2 4 6\n1 3 5\n", "");
    failed += row({"rotate", "90"}, "P3\n2 1\n255\n255 0 0 0 255 0\n", 0, "P3\n1 2\n255\n255 0 0\n0 255 0\n", "");
    failed += row({"rotate", "45"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 2, "", "pnmforge: rotate needs 90, 180 or 270\n");
    return failed != 0;
}
