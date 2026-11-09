#include "pnmforge.hpp"

#include <cstddef>
#include <vector>

namespace pnmforge {

std::string cmd_ascii(const Options &opts, const std::vector<std::string> &, const Image &image) {
    const std::vector<int> values = image.kind == P3 ? luma(image) : image.samples;
    const std::size_t width = static_cast<std::size_t>(image.width);
    const std::size_t ramp_length = opts.ramp.size();
    const std::size_t denominator = static_cast<std::size_t>(image.maxval);
    const std::size_t rounding = denominator / 2;
    std::string output;
    output.reserve(width * static_cast<std::size_t>(image.height + 1));

    for (int y = 0; y < image.height; ++y) {
        const std::size_t row_start = static_cast<std::size_t>(y) * width;
        for (std::size_t x = 0; x < width; ++x) {
            const std::size_t value = static_cast<std::size_t>(values[row_start + x]);
            const std::size_t index = (value * (ramp_length - 1) + rounding) / denominator;
            output.push_back(opts.ramp[index]);
        }
        output.push_back('\n');
    }
    return output;
}

} // namespace pnmforge
