#include "pnmforge.hpp"

namespace pnmforge {

const Command kCommands[] = {
    {"info", cmd_info, OPT_BRIEF, 0, 0, ""},
    {"histogram", cmd_histogram, OPT_BINS, 0, 0, ""},
    {"copy", cmd_copy, 0, 0, 0, ""},
    {"invert", cmd_invert, 0, 0, 0, ""},
    {"gray", cmd_gray, 0, 0, 0, ""},
    {"threshold", cmd_threshold, 0, 1, 1, "T"},
    {"levels", cmd_levels, 0, 2, 2, "LOW HIGH"},
    {"flip", cmd_flip, 0, 1, 1, "h|v"},
    {"rotate", cmd_rotate, 0, 1, 1, "90|180|270"},
    {"crop", cmd_crop, 0, 4, 4, "X Y W H"},
    {"scale", cmd_scale, 0, 2, 2, "up|down N"},
    {"convolve", cmd_convolve, OPT_EDGE, 1, 1, "blur|sharpen|edge|emboss"},
    {"dither", cmd_dither, 0, 0, 0, ""},
    {"components", cmd_components, OPT_EIGHT, 0, 0, ""},
    {"ascii", cmd_ascii, OPT_RAMP, 0, 0, ""},
    {nullptr, nullptr, 0, 0, 0, ""},
};

} // namespace pnmforge
