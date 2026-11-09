#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({"info"}, "", 1, "", "pnmforge: empty input\n");
    failed += row({"info"}, "  # only a comment\n", 1, "", "pnmforge: empty input\n");
    failed += row({"info"}, "P4 1 1\n0", 1, "", "pnmforge: unsupported format \"P4\"\n");
    failed += row({"info"}, "P2", 1, "", "pnmforge: truncated header\n");
    failed += row({"info"}, "P2 3", 1, "", "pnmforge: truncated header\n");
    failed += row({"info"}, "P2 x 2 255 0", 1, "", "pnmforge: bad width \"x\"\n");
    failed += row({"info"}, "P2 3 0 255", 1, "", "pnmforge: bad height \"0\"\n");
    failed += row({"info"}, "P2 1 1 256 0", 1, "", "pnmforge: bad maxval \"256\"\n");
    failed += row({"info"}, "P2 2 1 255 7 300", 1, "", "pnmforge: bad sample \"300\"\n");
    failed += row({"info"}, "P2 2 2 9 1 2 3", 1, "", "pnmforge: truncated data\n");
    failed += row({"info"}, "P2 1 1 9 1 2", 1, "", "pnmforge: trailing data\n");
    failed += row({"info"}, "P1 2 2 1 0 1 2 0", 1, "", "pnmforge: bad sample \"2\"\n");
    failed += row({"info"}, "P1 2 1 101", 1, "", "pnmforge: trailing data\n");
    return failed != 0;
}
