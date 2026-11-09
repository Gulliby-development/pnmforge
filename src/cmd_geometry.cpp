#include "pnmforge.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace pnmforge {
namespace {

Image transform(const Image &image, int width, int height, int mode) {
    Image result;
    result.kind = image.kind;
    result.width = width;
    result.height = height;
    result.maxval = image.maxval;
    const std::size_t channels = static_cast<std::size_t>(image.channels());
    const std::size_t out_width = static_cast<std::size_t>(width);
    const std::size_t out_height = static_cast<std::size_t>(height);
    result.samples.resize(out_width * out_height * channels);
    for (std::size_t y = 0; y < out_height; ++y) {
        for (std::size_t x = 0; x < out_width; ++x) {
            std::size_t source_x = x;
            std::size_t source_y = y;
            if (mode == 0) {
                source_x = static_cast<std::size_t>(image.width - 1) - x;
            } else if (mode == 1) {
                source_y = static_cast<std::size_t>(image.height - 1) - y;
            } else if (mode == 2) {
                source_x = y;
                source_y = static_cast<std::size_t>(image.height - 1) - x;
            } else if (mode == 3) {
                source_x = static_cast<std::size_t>(image.width - 1) - x;
                source_y = static_cast<std::size_t>(image.height - 1) - y;
            } else if (mode == 4) {
                source_x = static_cast<std::size_t>(image.width - 1) - y;
                source_y = x;
            }
            const std::size_t source = (source_y * static_cast<std::size_t>(image.width) + source_x) * channels;
            const std::size_t destination = (y * out_width + x) * channels;
            for (std::size_t channel = 0; channel < channels; ++channel) {
                result.samples[destination + channel] = image.samples[source + channel];
            }
        }
    }
    return result;
}

bool crop_number(const std::string &word, int &value) {
    if (word.empty() || word.size() > 4) {
        return false;
    }
    int parsed = 0;
    for (unsigned char ch : word) {
        if (ch < '0' || ch > '9') {
            return false;
        }
        parsed = parsed * 10 + static_cast<int>(ch - '0');
    }
    value = parsed;
    return true;
}

} // namespace

std::string cmd_flip(const Options &, const std::vector<std::string> &pos, const Image &image) {
    if (pos[0] != "h" && pos[0] != "v") {
        throw Failure{2, "flip needs h or v"};
    }
    return format_image(transform(image, image.width, image.height, pos[0] == "h" ? 0 : 1));
}

std::string cmd_rotate(const Options &, const std::vector<std::string> &pos, const Image &image) {
    int width = image.width;
    int height = image.height;
    int mode = 0;
    if (pos[0] == "90") {
        width = image.height;
        height = image.width;
        mode = 2;
    } else if (pos[0] == "180") {
        mode = 3;
    } else if (pos[0] == "270") {
        width = image.height;
        height = image.width;
        mode = 4;
    } else {
        throw Failure{2, "rotate needs 90, 180 or 270"};
    }
    return format_image(transform(image, width, height, mode));
}

std::string cmd_crop(const Options &, const std::vector<std::string> &pos, const Image &image) {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    if (!crop_number(pos[0], x) || !crop_number(pos[1], y) || !crop_number(pos[2], width) ||
        !crop_number(pos[3], height) || width == 0 || height == 0) {
        throw Failure{2, "crop needs non-negative integers X Y and positive W H"};
    }
    if (x + width > image.width || y + height > image.height) {
        throw Failure{1, "crop region outside the image"};
    }
    Image result;
    result.kind = image.kind;
    result.width = width;
    result.height = height;
    result.maxval = image.maxval;
    const std::size_t channels = static_cast<std::size_t>(image.channels());
    const std::size_t source_width = static_cast<std::size_t>(image.width);
    const std::size_t crop_width = static_cast<std::size_t>(width);
    const std::size_t crop_height = static_cast<std::size_t>(height);
    result.samples.resize(crop_width * crop_height * channels);
    for (std::size_t row = 0; row < crop_height; ++row) {
        const std::size_t source = (static_cast<std::size_t>(y) + row) * source_width + static_cast<std::size_t>(x);
        const std::size_t destination = row * crop_width;
        for (std::size_t column = 0; column < crop_width; ++column) {
            for (std::size_t channel = 0; channel < channels; ++channel) {
                result.samples[(destination + column) * channels + channel] =
                    image.samples[(source + column) * channels + channel];
            }
        }
    }
    return format_image(result);
}

} // namespace pnmforge
