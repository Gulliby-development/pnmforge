#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"threshold", "128"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "P1\n3 2\n1 0 0\n1 0 1\n", "");
    failed += row({"threshold", "0"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "P1\n3 2\n0 0 0\n0 0 0\n", "");
    failed += row({"threshold", "256"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 2, "", "pnmforge: threshold needs an integer from 0 to 255\n");
    failed += row({"threshold", "100"}, "P3 2 1 255\n255 0 0 0 255 0\n", 0, "P1\n2 1\n1 0\n", "");
    failed += row({"threshold", "x"}, "P1\n3 2\n1 0 1\n0 1 0\n", 1, "", "pnmforge: threshold needs a P2 or P3 image\n");
    failed += row({"levels", "50", "200"}, "P2 4 1 255\n0 50 125 255\n", 0, "P2\n4 1\n255\n0 0 128 255\n", "");
    failed += row({"levels", "100", "100"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 2, "", "pnmforge: levels needs LOW below HIGH, both from 0 to 255\n");
    failed += row({"levels", "0", "200"}, "P2 2 1 100\n0 100\n", 1, "", "pnmforge: levels HIGH above maxval 100\n");
    failed += row({"levels", "2", "7"}, "P3 1 1 9\n1 5 9\n", 0, "P3\n1 1\n9\n0 5 9\n", "");
    return failed != 0;
}
