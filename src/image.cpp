#include "pnmforge.hpp"

#include <cstddef>
#include <string>

namespace pnmforge {
namespace {

bool whitespace(unsigned char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v' || ch == '\f';
}

class Scanner {
  public:
    explicit Scanner(const std::string &input) : input_(input) {
    }

    bool next(std::string &token) {
        for (;;) {
            while (position_ < input_.size() && whitespace(static_cast<unsigned char>(input_[position_]))) {
                ++position_;
            }
            if (position_ == input_.size()) {
                return false;
            }
            if (input_[position_] != '#') {
                break;
            }
            while (position_ < input_.size() && input_[position_] != '\r' && input_[position_] != '\n') {
                ++position_;
            }
        }
        const std::size_t start = position_;
        while (position_ < input_.size() && !whitespace(static_cast<unsigned char>(input_[position_]))) {
            ++position_;
        }
        token.assign(input_, start, position_ - start);
        return true;
    }

  private:
    const std::string &input_;
    std::size_t position_ = 0;
};

bool decimal(const std::string &token, int low, int high, int &value) {
    if (token.empty() || token.size() > 6) {
        return false;
    }
    int parsed = 0;
    for (unsigned char ch : token) {
        if (ch < '0' || ch > '9') {
            return false;
        }
        parsed = parsed * 10 + static_cast<int>(ch - '0');
    }
    if (parsed < low || parsed > high) {
        return false;
    }
    value = parsed;
    return true;
}

std::string quoted_error(const char *prefix, const std::string &token) {
    return std::string(prefix) + " \"" + token + "\"";
}

void append_number(std::string &output, int value) {
    char digits[16];
    std::size_t length = 0;
    unsigned int number = static_cast<unsigned int>(value);
    do {
        digits[length++] = static_cast<char>('0' + number % 10u);
        number /= 10u;
    } while (number != 0u);
    while (length != 0) {
        output.push_back(digits[--length]);
    }
}

} // namespace

Image parse_image(const std::string &text) {
    Scanner scanner(text);
    std::string token;
    if (!scanner.next(token)) {
        throw Failure{1, "empty input"};
    }
    Image image;
    if (token == "P1") {
        image.kind = P1;
        image.maxval = 1;
    } else if (token == "P2") {
        image.kind = P2;
    } else if (token == "P3") {
        image.kind = P3;
    } else {
        throw Failure{1, quoted_error("unsupported format", token)};
    }

    if (!scanner.next(token)) {
        throw Failure{1, "truncated header"};
    }
    if (!decimal(token, 1, 1000, image.width)) {
        throw Failure{1, quoted_error("bad width", token)};
    }
    if (!scanner.next(token)) {
        throw Failure{1, "truncated header"};
    }
    if (!decimal(token, 1, 1000, image.height)) {
        throw Failure{1, quoted_error("bad height", token)};
    }
    if (image.kind != P1) {
        if (!scanner.next(token)) {
            throw Failure{1, "truncated header"};
        }
        if (!decimal(token, 1, 255, image.maxval)) {
            throw Failure{1, quoted_error("bad maxval", token)};
        }
    }

    const std::size_t pixels = static_cast<std::size_t>(image.width) * static_cast<std::size_t>(image.height);
    const std::size_t required = pixels * static_cast<std::size_t>(image.channels());
    image.samples.reserve(required);
    while (image.samples.size() < required) {
        if (!scanner.next(token)) {
            throw Failure{1, "truncated data"};
        }
        if (image.kind == P1) {
            for (unsigned char ch : token) {
                if (ch != '0' && ch != '1') {
                    throw Failure{1, quoted_error("bad sample", token)};
                }
            }
            if (token.size() > required - image.samples.size()) {
                throw Failure{1, "trailing data"};
            }
            for (char ch : token) {
                image.samples.push_back(ch == '1' ? 1 : 0);
            }
        } else {
            int sample = 0;
            if (!decimal(token, 0, image.maxval, sample)) {
                throw Failure{1, quoted_error("bad sample", token)};
            }
            image.samples.push_back(sample);
        }
    }
    if (scanner.next(token)) {
        throw Failure{1, "trailing data"};
    }
    return image;
}

std::string format_image(const Image &image) {
    std::string output;
    output.reserve(image.samples.size() * 4 + 32);
    output.push_back('P');
    output.push_back(static_cast<char>('0' + static_cast<int>(image.kind)));
    output.push_back('\n');
    append_number(output, image.width);
    output.push_back(' ');
    append_number(output, image.height);
    output.push_back('\n');
    if (image.kind != P1) {
        append_number(output, image.maxval);
        output.push_back('\n');
    }
    const std::size_t channels = static_cast<std::size_t>(image.channels());
    const std::size_t row_samples = static_cast<std::size_t>(image.width) * channels;
    for (int y = 0; y < image.height; ++y) {
        for (std::size_t x = 0; x < row_samples; ++x) {
            if (x != 0) {
                output.push_back(' ');
            }
            append_number(output, image.samples[static_cast<std::size_t>(y) * row_samples + x]);
        }
        output.push_back('\n');
    }
    return output;
}

std::vector<int> luma(const Image &image) {
    if (image.kind != P3) {
        return image.samples;
    }
    std::vector<int> result;
    const std::size_t pixels = image.samples.size() / 3;
    result.reserve(pixels);
    for (std::size_t i = 0; i < pixels; ++i) {
        const std::size_t offset = i * 3;
        result.push_back((299 * image.samples[offset] + 587 * image.samples[offset + 1] + 114 * image.samples[offset + 2] + 500) / 1000);
    }
    return result;
}

} // namespace pnmforge
