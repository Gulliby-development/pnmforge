#include <string>
#include <vector>

#include "pnmforge.hpp"

namespace pnmforge {
namespace {

bool decimal_argument(const std::string &text, int &value) {
    if (text.empty() || text.size() > 3) {
        return false;
    }
    int parsed = 0;
    for (unsigned char ch : text) {
        if (ch < '0' || ch > '9') {
            return false;
        }
        parsed = parsed * 10 + static_cast<int>(ch - '0');
    }
    if (parsed > 255) {
        return false;
    }
    value = parsed;
    return true;
}

void require_gray_kind(const Image &image, const char *command) {
    if (image.kind == P1) {
        throw Failure{1, std::string(command) + " needs a P2 or P3 image"};
    }
}

} // namespace

std::string cmd_invert(const Options &, const std::vector<std::string> &, const Image &image) {
    Image result = image;
    for (int &sample : result.samples) {
        sample = image.maxval - sample;
    }
    return format_image(result);
}

std::string cmd_gray(const Options &, const std::vector<std::string> &, const Image &image) {
    require_gray_kind(image, "gray");
    Image result = image;
    result.kind = P2;
    result.samples = luma(image);
    return format_image(result);
}

std::string cmd_threshold(const Options &, const std::vector<std::string> &pos, const Image &image) {
    require_gray_kind(image, "threshold");
    int threshold = 0;
    if (!decimal_argument(pos[0], threshold)) {
        throw Failure{2, "threshold needs an integer from 0 to 255"};
    }
    const std::vector<int> gray = luma(image);
    Image result;
    result.kind = P1;
    result.width = image.width;
    result.height = image.height;
    result.maxval = 1;
    result.samples.reserve(gray.size());
    for (int value : gray) {
        result.samples.push_back(value < threshold ? 1 : 0);
    }
    return format_image(result);
}

std::string cmd_levels(const Options &, const std::vector<std::string> &pos, const Image &image) {
    require_gray_kind(image, "levels");
    int low = 0;
    int high = 0;
    if (!decimal_argument(pos[0], low) || !decimal_argument(pos[1], high) || low >= high) {
        throw Failure{2, "levels needs LOW below HIGH, both from 0 to 255"};
    }
    if (high > image.maxval) {
        throw Failure{1, "levels HIGH above maxval " + std::to_string(image.maxval)};
    }
    Image result = image;
    const int range = high - low;
    for (int &sample : result.samples) {
        const int clamped = sample < low ? low : (sample > high ? high : sample);
        sample = ((clamped - low) * image.maxval + range / 2) / range;
    }
    return format_image(result);
}

} // namespace pnmforge
