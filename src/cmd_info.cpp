#include "pnmforge.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

namespace pnmforge {

std::string cmd_info(const Options &opts, const std::vector<std::string> &, const Image &image) {
    const std::string format = "P" + std::to_string(static_cast<int>(image.kind));
    const std::string size = std::to_string(image.width) + "x" + std::to_string(image.height);
    if (opts.brief) {
        return format + " " + size + " " + std::to_string(image.maxval) + "\n";
    }

    const int minimum = *std::min_element(image.samples.begin(), image.samples.end());
    const int maximum = *std::max_element(image.samples.begin(), image.samples.end());
    unsigned long long sum = 0;
    for (int sample : image.samples) {
        sum += static_cast<unsigned long long>(sample);
    }

    return "format " + format + "\nsize " + size + "\nmaxval " + std::to_string(image.maxval) +
           "\nsamples " + std::to_string(image.samples.size()) + "\nmin " + std::to_string(minimum) +
           "\nmax " + std::to_string(maximum) + "\nsum " + std::to_string(sum) + "\n";
}

std::string cmd_histogram(const Options &opts, const std::vector<std::string> &, const Image &image) {
    if (image.kind != P1 && image.kind != P2) {
        throw Failure{1, "histogram needs a P1 or P2 image"};
    }

    const int value_count = image.maxval + 1;
    const int bins = opts.bins == 0 ? std::min(8, value_count) : opts.bins;
    if (bins > value_count) {
        throw Failure{1, "too many bins for maxval " + std::to_string(image.maxval)};
    }

    std::vector<std::size_t> frequencies(static_cast<std::size_t>(value_count), 0);
    for (int sample : image.samples) {
        ++frequencies[static_cast<std::size_t>(sample)];
    }

    std::string output;
    for (int i = 0; i < bins; ++i) {
        const int low = i * value_count / bins;
        const int high = (i + 1) * value_count / bins - 1;
        std::size_t count = 0;
        for (int value = low; value <= high; ++value) {
            count += frequencies[static_cast<std::size_t>(value)];
        }
        output += std::to_string(low) + "-" + std::to_string(high) + " " + std::to_string(count) + "\n";
    }
    return output;
}

std::string cmd_copy(const Options &, const std::vector<std::string> &, const Image &image) {
    return format_image(image);
}

} // namespace pnmforge
