#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"crop", "1", "0", "2", "2"}, "P2 3 2 9\n1 2 3\n4 5 6\n", 0, "P2\n2 2\n9\n2 3\n5 6\n", "");
    failed += row({"crop", "0", "0", "3", "2"}, "P2 3 2 9\n1 2 3\n4 5 6\n", 0, "P2\n3 2\n9\n1 2 3\n4 5 6\n", "");
    failed += row({"crop", "1", "1", "1", "1"}, "P3\n2 2\n255\n255 0 0 0 255 0\n7 8 9 1 2 3\n", 0, "P3\n1 1\n255\n1 2 3\n", "");
    failed += row({"crop", "2", "0", "2", "1"}, "P2 3 2 9\n1 2 3\n4 5 6\n", 1, "", "pnmforge: crop region outside the image\n");
    failed += row({"crop", "0", "0", "0", "1"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 2, "", "pnmforge: crop needs non-negative integers X Y and positive W H\n");
    failed += row({"crop", "0", "x", "1", "1"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 2, "", "pnmforge: crop needs non-negative integers X Y and positive W H\n");
    failed += row({"crop", "00", "01", "2", "1"}, "P2\n2 3\n9\n1 2\n3 4\n5 6\n", 0, "P2\n2 1\n9\n3 4\n", "");
    return failed != 0;
}
