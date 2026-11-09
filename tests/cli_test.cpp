#include "harness.hpp"

int main() {
    int failed = 0;
    failed += row({}, "", 2, "", "usage: pnmforge info [--brief]\n       pnmforge histogram [--bins N]\n       pnmforge copy\n       pnmforge invert\n       pnmforge gray\n       pnmforge threshold T\n       pnmforge levels LOW HIGH\n       pnmforge flip h|v\n       pnmforge rotate 90|180|270\n       pnmforge crop X Y W H\n       pnmforge scale up|down N\n       pnmforge convolve [--edge clamp|zero|wrap] blur|sharpen|edge|emboss\n       pnmforge dither\n       pnmforge components [--eight]\n       pnmforge ascii [--ramp CHARS]\n");
    failed += row({"help"}, "", 0, "usage: pnmforge info [--brief]\n       pnmforge histogram [--bins N]\n       pnmforge copy\n       pnmforge invert\n       pnmforge gray\n       pnmforge threshold T\n       pnmforge levels LOW HIGH\n       pnmforge flip h|v\n       pnmforge rotate 90|180|270\n       pnmforge crop X Y W H\n       pnmforge scale up|down N\n       pnmforge convolve [--edge clamp|zero|wrap] blur|sharpen|edge|emboss\n       pnmforge dither\n       pnmforge components [--eight]\n       pnmforge ascii [--ramp CHARS]\n", "");
    failed += row({"--version"}, "", 0, "pnmforge 0.1.0\n", "");
    failed += row({"--version", "x"}, "", 0, "pnmforge 0.1.0\n", "");
    failed += row({"frob"}, "", 2, "", "pnmforge: unknown command frob\n");
    failed += row({""}, "", 2, "", "pnmforge: unknown command \n");
    failed += row({"Info"}, "", 2, "", "pnmforge: unknown command Info\n");
    failed += row({"--bins", "2"}, "", 2, "", "pnmforge: unknown command --bins\n");
    failed += row({"copy", "--version"}, "", 2, "", "pnmforge: unknown option --version\n");
    return failed != 0;
}
