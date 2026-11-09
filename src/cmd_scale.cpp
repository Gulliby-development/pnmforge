#include "pnmforge.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace pnmforge {
namespace {

bool scale_factor(const std::string &text, int &factor) {
    if (text.empty() || text.size() > 2) {
        return false;
    }
    int value = 0;
    for (unsigned char ch : text) {
        if (ch < '0' || ch > '9') {
            return false;
        }
        value = value * 10 + static_cast<int>(ch - '0');
    }
    if (value < 2 || value > 16) {
        return false;
    }
    factor = value;
    return true;
}

} // namespace

std::string cmd_scale(const Options &, const std::vector<std::string> &pos, const Image &image) {
    const std::string &direction = pos[0];
    if (direction != "up" && direction != "down") {
        throw Failure{2, "scale needs up or down"};
    }

    int factor = 0;
    if (!scale_factor(pos[1], factor)) {
        throw Failure{2, "scale needs a factor from 2 to 16"};
    }

    Image result;
    result.kind = image.kind;
    result.maxval = image.maxval;
    const int channels = image.channels();
    if (direction == "up") {
        result.width = image.width * factor;
        result.height = image.height * factor;
        const std::size_t output_width = static_cast<std::size_t>(result.width);
        const std::size_t output_channels = static_cast<std::size_t>(channels);
        const std::size_t output_size = output_width * static_cast<std::size_t>(result.height) * output_channels;
        result.samples.resize(output_size);
        const std::size_t input_width = static_cast<std::size_t>(image.width);
        const std::size_t input_channels = static_cast<std::size_t>(channels);
        for (int y = 0; y < result.height; ++y) {
            const std::size_t source_y = static_cast<std::size_t>(y / factor);
            for (int x = 0; x < result.width; ++x) {
                const std::size_t source_x = static_cast<std::size_t>(x / factor);
                const std::size_t source = (source_y * input_width + source_x) * input_channels;
                const std::size_t destination = (static_cast<std::size_t>(y) * output_width + static_cast<std::size_t>(x)) * output_channels;
                for (int channel = 0; channel < channels; ++channel) {
                    result.samples[destination + static_cast<std::size_t>(channel)] = image.samples[source + static_cast<std::size_t>(channel)];
                }
            }
        }
    } else {
        result.width = image.width / factor;
        result.height = image.height / factor;
        if (result.width == 0 || result.height == 0) {
            throw Failure{1, "image too small to scale down by " + std::to_string(factor)};
        }
        const std::size_t output_width = static_cast<std::size_t>(result.width);
        const std::size_t output_height = static_cast<std::size_t>(result.height);
        const std::size_t output_channels = static_cast<std::size_t>(channels);
        result.samples.resize(output_width * output_height * output_channels);
        const std::size_t input_width = static_cast<std::size_t>(image.width);
        const std::size_t input_channels = static_cast<std::size_t>(channels);
        const int block_area = factor * factor;
        for (int y = 0; y < result.height; ++y) {
            for (int x = 0; x < result.width; ++x) {
                for (int channel = 0; channel < channels; ++channel) {
                    int sum = 0;
                    for (int dy = 0; dy < factor; ++dy) {
                        for (int dx = 0; dx < factor; ++dx) {
                            const std::size_t source_x = static_cast<std::size_t>(x * factor + dx);
                            const std::size_t source_y = static_cast<std::size_t>(y * factor + dy);
                            const std::size_t source = (source_y * input_width + source_x) * input_channels + static_cast<std::size_t>(channel);
                            sum += image.samples[source];
                        }
                    }
                    const std::size_t destination = (static_cast<std::size_t>(y) * output_width + static_cast<std::size_t>(x)) * output_channels + static_cast<std::size_t>(channel);
                    result.samples[destination] = (sum + block_area / 2) / block_area;
                }
            }
        }
    }
    return format_image(result);
}

} // namespace pnmforge
