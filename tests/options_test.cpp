#include "harness.hpp"

int main() {
    int failed = 0;
    failed += reads({"info"}, true);
    failed += reads({}, false);
    failed += reads({"info", "x"}, false);
    failed += reads({"histogram", "--bins", "3"}, true);
    failed += reads({"histogram", "--bins", "0"}, false);
    failed += reads({"crop", "1", "2", "3", "4"}, true);
    failed += row({"histogram", "--bins"}, "", 2, "", "pnmforge: --bins needs an integer from 1 to 256\n");
    failed += row({"histogram", "--bins", "257"}, "", 2, "", "pnmforge: --bins needs an integer from 1 to 256\n");
    failed += row({"convolve", "--edge", "mirror", "blur"}, "", 2, "", "pnmforge: --edge needs one of clamp, zero, wrap\n");
    failed += row({"ascii", "--ramp", "a"}, "", 2, "", "pnmforge: --ramp needs 2 to 64 printable characters\n");
    failed += row({"info", "--brief", "--brief"}, "", 2, "", "pnmforge: --brief given twice\n");
    failed += row({"copy", "--brief"}, "", 2, "", "pnmforge: unknown option --brief\n");
    failed += row({"info", "extra"}, "", 2, "", "pnmforge: unexpected argument extra\n");
    failed += row({"crop", "1", "2"}, "", 2, "", "pnmforge: crop needs X Y W H\n");
    return failed != 0;
}
