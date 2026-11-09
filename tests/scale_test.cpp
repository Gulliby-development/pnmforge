#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"scale", "up", "2"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 0, "P2\n4 6\n9\n1 1 2 2\n1 1 2 2\n3 3 4 4\n3 3 4 4\n5 5 6 6\n5 5 6 6\n", "");
    failed += row({"scale", "up", "3"}, "P1\n2 1\n1 0\n", 0, "P1\n6 3\n1 1 1 0 0 0\n1 1 1 0 0 0\n1 1 1 0 0 0\n", "");
    failed += row({"scale", "up", "2"}, "P3 1 1 255\n1 2 3\n", 0, "P3\n2 2\n255\n1 2 3 1 2 3\n1 2 3 1 2 3\n", "");
    failed += row({"scale", "down", "2"}, "P2 4 2 255\n10 20 30 41\n0 3 5 6\n", 0, "P2\n2 1\n255\n8 21\n", "");
    failed += row({"scale", "down", "2"}, "P1 4 2 1\n1 1 0 1\n1 0 0 0\n", 1, "", "pnmforge: trailing data\n");
    failed += row({"scale", "down", "4"}, "P2 3 3 9\n1 1 1\n1 1 1\n1 1 1\n", 1, "", "pnmforge: image too small to scale down by 4\n");
    failed += row({"scale", "sideways", "2"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 2, "", "pnmforge: scale needs up or down\n");
    failed += row({"scale", "up", "1"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 2, "", "pnmforge: scale needs a factor from 2 to 16\n");
    return failed != 0;
}
