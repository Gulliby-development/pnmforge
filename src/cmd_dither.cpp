#include "pnmforge.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

namespace pnmforge {
namespace {

std::size_t pixel_index(int x, int y, int width) {
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
}

} // namespace

std::string cmd_dither(const Options &, const std::vector<std::string> &, const Image &image) {
    if (image.kind == P1) {
        throw Failure{1, "dither needs a P2 or P3 image"};
    }

    const std::vector<int> gray = luma(image);
    const std::size_t pixel_count = gray.size();
    std::vector<int> errors(pixel_count, 0);
    Image output;
    output.kind = P1;
    output.width = image.width;
    output.height = image.height;
    output.maxval = 1;
    output.samples.resize(pixel_count);

    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            const std::size_t index = pixel_index(x, y, image.width);
            const int old = gray[index] + errors[index];
            const int bit = 2 * old >= image.maxval ? 0 : 1;
            output.samples[index] = bit;
            const int error = old - (bit == 0 ? image.maxval : 0);
            const int share7 = error * 7 / 16;
            const int share3 = error * 3 / 16;
            const int share5 = error * 5 / 16;
            const int share_remainder = error - share7 - share3 - share5;

            if (x + 1 < image.width) {
                errors[pixel_index(x + 1, y, image.width)] += share7;
            }
            if (y + 1 < image.height) {
                if (x > 0) {
                    errors[pixel_index(x - 1, y + 1, image.width)] += share3;
                }
                errors[pixel_index(x, y + 1, image.width)] += share5;
                if (x + 1 < image.width) {
                    errors[pixel_index(x + 1, y + 1, image.width)] += share_remainder;
                }
            }
        }
    }
    return format_image(output);
}

std::string cmd_components(const Options &opts, const std::vector<std::string> &, const Image &image) {
    if (image.kind != P1) {
        throw Failure{1, "components needs a P1 image"};
    }

    const std::size_t pixel_count = image.samples.size();
    std::vector<unsigned char> visited(pixel_count, 0);
    std::vector<std::size_t> pending;
    std::vector<std::size_t> sizes;

    for (std::size_t start = 0; start < pixel_count; ++start) {
        if (image.samples[start] == 0 || visited[start] != 0) {
            continue;
        }
        visited[start] = 1;
        pending.clear();
        pending.push_back(start);
        std::size_t component_size = 0;
        for (std::size_t next = 0; next < pending.size(); ++next) {
            const std::size_t current = pending[next];
            ++component_size;
            const int x = static_cast<int>(current % static_cast<std::size_t>(image.width));
            const int y = static_cast<int>(current / static_cast<std::size_t>(image.width));
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    if (!opts.eight && dx != 0 && dy != 0) {
                        continue;
                    }
                    const int neighbor_x = x + dx;
                    const int neighbor_y = y + dy;
                    if (neighbor_x < 0 || neighbor_x >= image.width || neighbor_y < 0 || neighbor_y >= image.height) {
                        continue;
                    }
                    const std::size_t neighbor = pixel_index(neighbor_x, neighbor_y, image.width);
                    if (image.samples[neighbor] != 0 && visited[neighbor] == 0) {
                        visited[neighbor] = 1;
                        pending.push_back(neighbor);
                    }
                }
            }
        }
        sizes.push_back(component_size);
    }

    std::sort(sizes.begin(), sizes.end(), std::greater<std::size_t>());
    std::string output = "components " + std::to_string(sizes.size()) + "\nsizes";
    for (std::size_t size : sizes) {
        output += " " + std::to_string(size);
    }
    output.push_back('\n');
    return output;
}

} // namespace pnmforge
