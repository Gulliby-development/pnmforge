#include "pnmforge.hpp"

#include <cstddef>
#include <exception>
#include <string>
#include <vector>

namespace pnmforge {
namespace {

const char kUsage[] =
    "usage: pnmforge info [--brief]\n"
    "       pnmforge histogram [--bins N]\n"
    "       pnmforge copy\n"
    "       pnmforge invert\n"
    "       pnmforge gray\n"
    "       pnmforge threshold T\n"
    "       pnmforge levels LOW HIGH\n"
    "       pnmforge flip h|v\n"
    "       pnmforge rotate 90|180|270\n"
    "       pnmforge crop X Y W H\n"
    "       pnmforge scale up|down N\n"
    "       pnmforge convolve [--edge clamp|zero|wrap] blur|sharpen|edge|emboss\n"
    "       pnmforge dither\n"
    "       pnmforge components [--eight]\n"
    "       pnmforge ascii [--ramp CHARS]\n";

struct Parsed {
    const Command *command = nullptr;
    Options options;
    std::vector<std::string> positionals;
    std::string error;
};

const Command *find_command(const std::string &name) {
    for (const Command *command = kCommands; command->name != nullptr; ++command) {
        if (name == command->name) {
            return command;
        }
    }
    return nullptr;
}

bool decimal(const std::string &value, std::size_t min_length, std::size_t max_length, int low, int high, int &number) {
    if (value.size() < min_length || value.size() > max_length) {
        return false;
    }
    int parsed = 0;
    for (unsigned char ch : value) {
        if (ch < '0' || ch > '9') {
            return false;
        }
        parsed = parsed * 10 + static_cast<int>(ch - '0');
    }
    if (parsed < low || parsed > high) {
        return false;
    }
    number = parsed;
    return true;
}

Parsed validate(const std::vector<std::string> &args) {
    Parsed parsed;
    if (args.empty()) {
        parsed.error = "missing command";
        return parsed;
    }
    parsed.command = find_command(args[0]);
    if (parsed.command == nullptr) {
        parsed.error = "unknown command " + args[0];
        return parsed;
    }

    unsigned seen = 0;
    for (std::size_t i = 1; i < args.size(); ++i) {
        const std::string &word = args[i];
        if (word.empty() || word[0] != '-' || word == "-") {
            if (parsed.positionals.size() >= static_cast<std::size_t>(parsed.command->max_args)) {
                parsed.error = "unexpected argument " + word;
                return parsed;
            }
            parsed.positionals.push_back(word);
            continue;
        }

        unsigned bit = 0;
        if (word == "--brief") {
            bit = OPT_BRIEF;
        } else if (word == "--bins") {
            bit = OPT_BINS;
        } else if (word == "--edge") {
            bit = OPT_EDGE;
        } else if (word == "--eight") {
            bit = OPT_EIGHT;
        } else if (word == "--ramp") {
            bit = OPT_RAMP;
        }
        if (bit == 0 || (parsed.command->options & bit) == 0) {
            parsed.error = "unknown option " + word;
            return parsed;
        }
        if ((seen & bit) != 0) {
            parsed.error = word + " given twice";
            return parsed;
        }
        seen |= bit;

        if (bit == OPT_BRIEF) {
            parsed.options.brief = true;
        } else if (bit == OPT_EIGHT) {
            parsed.options.eight = true;
        } else {
            if (i + 1 >= args.size()) {
                if (bit == OPT_BINS) {
                    parsed.error = "--bins needs an integer from 1 to 256";
                } else if (bit == OPT_EDGE) {
                    parsed.error = "--edge needs one of clamp, zero, wrap";
                } else {
                    parsed.error = "--ramp needs 2 to 64 printable characters";
                }
                return parsed;
            }
            const std::string &value = args[++i];
            if (bit == OPT_BINS) {
                int bins = 0;
                if (!decimal(value, 1, 3, 1, 256, bins)) {
                    parsed.error = "--bins needs an integer from 1 to 256";
                    return parsed;
                }
                parsed.options.bins = bins;
            } else if (bit == OPT_EDGE) {
                if (value != "clamp" && value != "zero" && value != "wrap") {
                    parsed.error = "--edge needs one of clamp, zero, wrap";
                    return parsed;
                }
                parsed.options.edge = value;
            } else {
                bool valid = value.size() >= 2 && value.size() <= 64;
                for (unsigned char ch : value) {
                    if (ch < 32 || ch > 126) {
                        valid = false;
                    }
                }
                if (!valid) {
                    parsed.error = "--ramp needs 2 to 64 printable characters";
                    return parsed;
                }
                parsed.options.ramp = value;
            }
        }
    }

    if (parsed.positionals.size() < static_cast<std::size_t>(parsed.command->min_args)) {
        parsed.error = std::string(parsed.command->name) + " needs " + parsed.command->arg_names;
    }
    return parsed;
}

Result cli_error(const std::string &message) {
    return Result{2, "", "pnmforge: " + message + "\n"};
}

} // namespace

bool reads_stdin(const std::vector<std::string> &args) {
    if (args.empty() || args[0] == "help" || args[0] == "-h" || args[0] == "--help" || args[0] == "--version") {
        return false;
    }
    try {
        return validate(args).error.empty();
    } catch (...) {
        return false;
    }
}

Result run(const std::vector<std::string> &args, const std::string &input) {
    if (args.empty()) {
        return Result{2, "", kUsage};
    }
    if (args[0] == "help" || args[0] == "-h" || args[0] == "--help") {
        return Result{0, kUsage, ""};
    }
    if (args[0] == "--version") {
        return Result{0, std::string("pnmforge ") + kVersion + "\n", ""};
    }
    try {
        Parsed parsed = validate(args);
        if (!parsed.error.empty()) {
            return cli_error(parsed.error);
        }
        Image image = parse_image(input);
        return Result{0, parsed.command->run(parsed.options, parsed.positionals, image), ""};
    } catch (const Failure &failure) {
        return Result{failure.status, "", "pnmforge: " + failure.message + "\n"};
    } catch (const std::exception &) {
        return Result{1, "", "pnmforge: internal error\n"};
    } catch (...) {
        return Result{1, "", "pnmforge: internal error\n"};
    }
}

} // namespace pnmforge
