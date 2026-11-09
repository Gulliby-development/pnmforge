#include "pnmforge.hpp"

#include <cstddef>
#include <vector>

namespace pnmforge {
namespace {

int floor_divide(int numerator, int denominator) {
    int quotient = numerator / denominator;
    if (numerator % denominator < 0) {
        --quotient;
    }
    return quotient;
}

int wrap_coordinate(int coordinate, int extent) {
    const int remainder = coordinate % extent;
    return remainder < 0 ? remainder + extent : remainder;
}

} // namespace

std::string cmd_convolve(const Options &opts, const std::vector<std::string> &pos, const Image &image) {
    if (image.kind != P2 && image.kind != P3) {
        throw Failure{1, "convolve needs a P2 or P3 image"};
    }

    int kernel[3][3] = {};
    int divisor = 1;
    if (pos[0] == "blur") {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                kernel[y][x] = 1;
            }
        }
        divisor = 9;
    } else if (pos[0] == "sharpen") {
        kernel[0][1] = -1;
        kernel[1][0] = -1;
        kernel[1][1] = 5;
        kernel[1][2] = -1;
        kernel[2][1] = -1;
    } else if (pos[0] == "edge") {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                kernel[y][x] = -1;
            }
        }
        kernel[1][1] = 8;
    } else if (pos[0] == "emboss") {
        const int emboss[3][3] = {{-2, -1, 0}, {-1, 1, 1}, {0, 1, 2}};
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                kernel[y][x] = emboss[y][x];
            }
        }
    } else {
        throw Failure{2, "convolve needs blur, sharpen, edge or emboss"};
    }

    Image output = image;
    const int channels = image.channels();
    const std::size_t row_samples = static_cast<std::size_t>(image.width) * static_cast<std::size_t>(channels);
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            for (int channel = 0; channel < channels; ++channel) {
                int weighted_sum = 0;
                for (int ky = 0; ky < 3; ++ky) {
                    for (int kx = 0; kx < 3; ++kx) {
                        int sample = 0;
                        int sx = x + kx - 1;
                        int sy = y + ky - 1;
                        if (opts.edge == "clamp") {
                            if (sx < 0) {
                                sx = 0;
                            } else if (sx >= image.width) {
                                sx = image.width - 1;
                            }
                            if (sy < 0) {
                                sy = 0;
                            } else if (sy >= image.height) {
                                sy = image.height - 1;
                            }
                            const std::size_t index = static_cast<std::size_t>(sy) * row_samples +
                                                      static_cast<std::size_t>(sx) * static_cast<std::size_t>(channels) +
                                                      static_cast<std::size_t>(channel);
                            sample = image.samples[index];
                        } else if (opts.edge == "wrap") {
                            sx = wrap_coordinate(sx, image.width);
                            sy = wrap_coordinate(sy, image.height);
                            const std::size_t index = static_cast<std::size_t>(sy) * row_samples +
                                                      static_cast<std::size_t>(sx) * static_cast<std::size_t>(channels) +
                                                      static_cast<std::size_t>(channel);
                            sample = image.samples[index];
                        } else if (sx >= 0 && sx < image.width && sy >= 0 && sy < image.height) {
                            const std::size_t index = static_cast<std::size_t>(sy) * row_samples +
                                                      static_cast<std::size_t>(sx) * static_cast<std::size_t>(channels) +
                                                      static_cast<std::size_t>(channel);
                            sample = image.samples[index];
                        }
                        weighted_sum += kernel[ky][kx] * sample;
                    }
                }
                const int rounded = floor_divide(weighted_sum + divisor / 2, divisor);
                const int value = rounded < 0 ? 0 : (rounded > image.maxval ? image.maxval : rounded);
                const std::size_t index = static_cast<std::size_t>(y) * row_samples +
                                          static_cast<std::size_t>(x) * static_cast<std::size_t>(channels) +
                                          static_cast<std::size_t>(channel);
                output.samples[index] = value;
            }
        }
    }
    return format_image(output);
}

} // namespace pnmforge
