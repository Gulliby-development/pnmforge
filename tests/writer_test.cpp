#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"copy"}, "P2 # width follows\n2 2\n# maxval\n255 1 2\n3 4\n", 0, "P2\n2 2\n255\n1 2\n3 4\n", "");
    failed += row({"copy"}, "P1 3 2 101 010", 0, "P1\n3 2\n1 0 1\n0 1 0\n", "");
    failed += row({"copy"}, "P1\r\n2 2\r\n1 0\r\n0 1\r\n", 0, "P1\n2 2\n1 0\n0 1\n", "");
    failed += row({"copy"}, "P3 1 2 255 1 2 3 4 5 6", 0, "P3\n1 2\n255\n1 2 3\n4 5 6\n", "");
    failed += row({"copy"}, "P3\n2 1\n255\n1 2 3\n4 5 6\n", 0, "P3\n2 1\n255\n1 2 3 4 5 6\n", "");
    failed += row({"copy"}, "P2 2 1 255 007 0010", 0, "P2\n2 1\n255\n7 10\n", "");
    failed += row({"copy"}, "P2\t1\t1\t255\t9", 0, "P2\n1 1\n255\n9\n", "");
    failed += row({"copy"}, "P2 2 1 1 0 1", 0, "P2\n2 1\n1\n0 1\n", "");
    return failed != 0;
}