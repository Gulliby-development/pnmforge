#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"dither"}, "P2 4 2 255\n0 64 128 255\n255 200 100 0\n", 0, "P1\n4 2\n1 1 0 0\n0 0 1 1\n", "");
    failed += row({"dither"}, "P2 3 1 255\n255 255 255\n", 0, "P1\n3 1\n0 0 0\n", "");
    failed += row({"dither"}, "P3 2 1 255\n255 0 0 0 255 0\n", 0, "P1\n2 1\n1 0\n", "");
    failed += row({"dither"}, "P1\n3 2\n1 0 1\n0 1 0\n", 1, "", "pnmforge: dither needs a P2 or P3 image\n");
    failed += row({"components"}, "P1 5 3 1\n1 1 0 1 0\n0 0 0 1 1\n1 0 0 0 0\n", 1, "", "pnmforge: trailing data\n");
    failed += row({"components", "--eight"}, "P1 5 3 1\n1 1 0 1 0\n0 0 0 1 1\n1 0 0 0 0\n", 1, "", "pnmforge: trailing data\n");
    failed += row({"components"}, "P1 2 1 1\n0 0\n", 1, "", "pnmforge: trailing data\n");
    failed += row({"components"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 1, "", "pnmforge: components needs a P1 image\n");
    failed += row({"components", "--eight"}, "P1 3 3 1\n1 0 1\n0 1 0\n1 0 1\n", 1, "", "pnmforge: trailing data\n");
    return failed != 0;
}
