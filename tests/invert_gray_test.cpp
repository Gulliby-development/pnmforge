#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"invert"}, "P2\n3 2\n255\n0 128 255\n64 192 32\n", 0, "P2\n3 2\n255\n255 127 0\n191 63 223\n", "");
    failed += row({"invert"}, "P1\n3 2\n1 0 1\n0 1 0\n", 0, "P1\n3 2\n0 1 0\n1 0 1\n", "");
    failed += row({"invert"}, "P3\n2 1\n255\n255 0 0 0 255 0\n", 0, "P3\n2 1\n255\n0 255 255 255 0 255\n", "");
    failed += row({"invert"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 0, "P2\n2 3\n9\n8 7\n6 5\n4 3\n", "");
    failed += row({"gray"}, "P3\n2 1\n255\n255 0 0 0 255 0\n", 0, "P2\n2 1\n255\n76 150\n", "");
    failed += row({"gray"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 0, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", "");
    failed += row({"gray"}, "P3 3 1 255\n255 0 0 0 255 0 0 0 255\n", 0, "P2\n3 1\n255\n76 150 29\n", "");
    failed += row({"gray"}, "P3 2 1 255\n255 255 255 1 1 1\n", 0, "P2\n2 1\n255\n255 1\n", "");
    failed += row({"gray"}, "P3 1 1 9\n9 4 1\n", 0, "P2\n1 1\n9\n5\n", "");
    return failed != 0;
}
