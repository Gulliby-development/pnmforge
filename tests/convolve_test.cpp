#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"convolve", "blur"}, "P2\n3 3\n255\n0 0 0\n0 90 0\n0 0 0\n", 0, "P2\n3 3\n255\n10 10 10\n10 10 10\n10 10 10\n", "");
    failed += row({"convolve", "sharpen"}, "P2\n3 3\n255\n0 0 0\n0 90 0\n0 0 0\n", 0, "P2\n3 3\n255\n0 0 0\n0 255 0\n0 0 0\n", "");
    failed += row({"convolve", "edge"}, "P2\n3 3\n255\n0 0 0\n0 90 0\n0 0 0\n", 0, "P2\n3 3\n255\n0 0 0\n0 255 0\n0 0 0\n", "");
    failed += row({"convolve", "emboss"}, "P2\n3 3\n255\n0 0 0\n0 90 0\n0 0 0\n", 0, "P2\n3 3\n255\n180 90 0\n90 90 0\n0 0 0\n", "");
    failed += row({"convolve", "--edge", "zero", "blur"}, "P2 3 1 90\n90 90 90\n", 0, "P2\n3 1\n90\n20 30 20\n", "");
    failed += row({"convolve", "--edge", "wrap", "blur"}, "P2 3 1 90\n90 0 0\n", 0, "P2\n3 1\n90\n30 30 30\n", "");
    failed += row({"convolve", "blur"}, "P3 2 1 255\n9 18 27 0 0 0\n", 0, "P3\n2 1\n255\n6 12 18 3 6 9\n", "");
    failed += row({"convolve", "blur"}, "P1\n3 2\n1 0 1\n0 1 0\n", 1, "", "pnmforge: convolve needs a P2 or P3 image\n");
    failed += row({"convolve", "sharpen"}, "P2 3 3 100\n10 10 10\n10 90 10\n10 10 10\n", 0, "P2\n3 3\n100\n10 0 10\n0 100 0\n10 0 10\n", "");
    return failed != 0;
}
