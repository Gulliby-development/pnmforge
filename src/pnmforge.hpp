#ifndef PNMFORGE_HPP
#define PNMFORGE_HPP

#include <string>
#include <vector>

namespace pnmforge {

inline constexpr const char *kVersion = "0.1.0";

enum Kind {
    P1 = 1,
    P2 = 2,
    P3 = 3,
};

/* A plain PNM image: samples are row-major, a P3 pixel holds three consecutive samples, a P1 sample is 0 or 1. */
struct Image {
    Kind kind = P2;
    int width = 0;
    int height = 0;
    int maxval = 1;
    std::vector<int> samples;
    int channels() const {
        return kind == P3 ? 3 : 1;
    }
};

/* What one command line produced: the exit status and the text for standard output and standard error. */
struct Result {
    int status = 0;
    std::string out;
    std::string err;
};

/* The options of a command line after the command word; bins is 0 until --bins is given. */
struct Options {
    bool brief = false;
    bool eight = false;
    int bins = 0;
    std::string edge = "clamp";
    std::string ramp = "@%#*+=-:. ";
};

/* Thrown by a command or the image reader: the exit status and the message without "pnmforge: " and the line feed. */
struct Failure {
    int status;
    std::string message;
};

/* A command returns the text for standard output or throws Failure. pos holds the positional words. */
using CommandFn = std::string (*)(const Options &opts, const std::vector<std::string> &pos, const Image &image);

constexpr unsigned OPT_BRIEF = 1u;
constexpr unsigned OPT_BINS = 2u;
constexpr unsigned OPT_EDGE = 4u;
constexpr unsigned OPT_EIGHT = 8u;
constexpr unsigned OPT_RAMP = 16u;

struct Command {
    const char *name;
    CommandFn run;
    unsigned options;
    int min_args;
    int max_args;
    const char *arg_names;
};

/* The command table, in the order of the usage text; the last entry has name nullptr. */
extern const Command kCommands[];

/* Runs a command line (the words after the program name) on the given standard input. */
Result run(const std::vector<std::string> &args, const std::string &input);

/* Whether the command line is a valid use of a command that reads standard input. */
bool reads_stdin(const std::vector<std::string> &args);

/* Reads a plain PNM image (P1, P2 or P3) or throws Failure; writes the canonical text of an image. */
Image parse_image(const std::string &text);
std::string format_image(const Image &image);

/* The P2 samples of a P2 or P3 image: P3 pixels become (299 R + 587 G + 114 B + 500) / 1000. */
std::vector<int> luma(const Image &image);

std::string cmd_info(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_histogram(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_copy(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_invert(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_gray(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_threshold(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_levels(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_flip(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_rotate(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_crop(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_scale(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_convolve(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_dither(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_components(const Options &opts, const std::vector<std::string> &pos, const Image &image);
std::string cmd_ascii(const Options &opts, const std::vector<std::string> &pos, const Image &image);

} // namespace pnmforge

#endif
