<p align="center"><img src="docs/logo.svg" width="96" alt="" /></p>

# pnmforge

[![CI](https://github.com/Gulliby-development/pnmforge/actions/workflows/ci.yml/badge.svg)](https://github.com/Gulliby-development/pnmforge/actions/workflows/ci.yml) [![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

pnmforge is a small C++17 command-line filter for reading and transforming plain Netpbm images. It reads one P1 bitmap, P2 graymap, or P3 pixmap from standard input and writes a canonical image or text report to standard output. It is intended for image-code developers, teachers, and anyone preparing small reproducible image fixtures or investigating image-processing behavior. It uses only the C++ standard library and has no image-library dependency.

## Build

Requirements: CMake 3.16 or newer and a C++17 compiler (MSVC, GCC, or Clang). The project is intended to build on Windows, macOS, and Linux.

~~~sh
cmake -S . -B build
cmake --build build --config Debug
~~~

The executable is <code>build/pnmforge</code> on typical single-configuration Unix generators, and <code>build/Debug/pnmforge.exe</code> with the Visual Studio generator on Windows. Use the executable path produced by your generator.

## Command synopsis

~~~text
usage: pnmforge info [--brief]
       pnmforge histogram [--bins N]
       pnmforge copy
       pnmforge invert
       pnmforge gray
       pnmforge threshold T
       pnmforge levels LOW HIGH
       pnmforge flip h|v
       pnmforge rotate 90|180|270
       pnmforge crop X Y W H
       pnmforge scale up|down N
       pnmforge convolve [--edge clamp|zero|wrap] blur|sharpen|edge|emboss
       pnmforge dither
       pnmforge components [--eight]
       pnmforge ascii [--ramp CHARS]
~~~

The first word selects a command. <code>help</code>, <code>-h</code>, and <code>--help</code> print the synopsis; <code>--version</code> prints <code>pnmforge 0.1.0</code>. With no word the synopsis is written to standard error and the status is 2.

## Input and output

A successful command consumes all of standard input as one image and writes its result to standard output, with no diagnostic output. Image output is canonical plain PNM: a magic line (<code>P1</code>, <code>P2</code>, or <code>P3</code>), a width and height line, a maxval line for P2/P3, then one row per line with samples separated by single ASCII spaces and no trailing spaces. Every line ends in LF. Comments and original formatting are not preserved. Transformed dimensions are not restricted by the input parser's dimension limit.

Whitespace in the input is space, tab, LF, CR, vertical tab, or form feed. A token is a maximal non-whitespace byte sequence. A <code>#</code> encountered where a token would begin starts a comment through the end of its line. The input must contain exactly one complete image; any additional token is trailing data.

Widths and heights are decimal strings of 1–6 digits with values 1–1000. P2/P3 maxval is 1–6 decimal digits with value 1–255. P2/P3 samples are 1–6 decimal digits from 0 through maxval. P1 has implicit maxval 1; its samples may be individual <code>0</code>/<code>1</code> tokens or packed together in a token. A non-bit in a P1 sample token is an error.

All commands that emit images use the canonical format. P3 stores red, green, and blue samples consecutively for each pixel. The gray value used for P3 operations is rounded luminance:
<code>floor((299 R + 587 G + 114 B + 500) / 1000)</code>.

## Commands

- **info [--brief]** — Works on any kind. Full output reports format, dimensions, maxval, sample count, minimum, maximum, and sum. Brief output is <code>FORMAT WIDTHxHEIGHT MAXVAL</code>.
- **histogram [--bins N]** — P1/P2 only. Default bin count is 8, or maxval + 1 when that is smaller. Bin <code>i</code> covers <code>LO=floor(i*(MAXVAL+1)/N)</code> through <code>HI=floor((i+1)*(MAXVAL+1)/N)-1</code>; output is <code>LO-HI COUNT</code> per bin. More than maxval + 1 bins fails.
- **copy** — Writes the input image in canonical form.
- **invert** — Any kind; each sample <code>v</code> becomes <code>MAXVAL-v</code>.
- **gray** — P2/P3 only. P2 is unchanged; P3 becomes P2 with rounded luminance samples.
- **threshold T** — P2/P3 only. T is 1–3 decimal digits, 0–255. Emits P1: a pixel is 1 when its gray value is below T, otherwise 0.
- **levels LOW HIGH** — P2/P3 only. Both are 1–3 decimal digits from 0–255, with LOW < HIGH; HIGH must not exceed image maxval. Clamp each input sample to LOW..HIGH, then map it to <code>floor(((v-LOW)*MAXVAL + floor((HIGH-LOW)/2))/(HIGH-LOW))</code>.
- **flip h|v** — Any kind. <code>h</code> reverses each row; <code>v</code> reverses row order. RGB pixels remain intact.
- **rotate 90|180|270** — Any kind; rotates clockwise. For 90/270, output width and height exchange places.
- **crop X Y W H** — Any kind. Coordinates are zero-based; X/Y are 1–4 decimal digits and W/H are positive 1–4 decimal digits. The requested rectangle must fit inside the image.
- **scale up|down N** — Any kind; N is 1–2 decimal digits in 2–16. Up repeats every pixel N times in each dimension. Down averages each complete N×N block per channel using <code>floor((sum+floor(N*N/2))/(N*N))</code>; incomplete right/bottom borders are dropped. A zero-width or zero-height result fails.
- **convolve [--edge MODE] KERNEL** — P2/P3 only. MODE is <code>clamp</code> (default), <code>zero</code>, or <code>wrap</code>: clamp uses the nearest in-image pixel, zero supplies a zero sample, and wrap repeats the image in both axes. Kernels are 3×3: blur is all ones with divisor 9; sharpen is <code>0 -1 0 / -1 5 -1 / 0 -1 0</code>; edge is <code>-1 -1 -1 / -1 8 -1 / -1 -1 -1</code>; emboss is <code>-2 -1 0 / -1 1 1 / 0 1 2</code>. For each channel independently, add floor(divisor/2) to the weighted sum, floor-divide by divisor (mathematical floor, including negative values), then clamp to 0..maxval.
- **dither** — P2/P3 only. Applies Floyd–Steinberg error diffusion in row-major order. A pixel is white (0) when twice its adjusted gray value is at least maxval, otherwise black (1). Error is distributed right/down-left/down/down-right in weights 7/16, 3/16, 5/16, and the remainder; integer shares truncate toward zero, and shares beyond the image are dropped. Emits P1.
- **components [--eight]** — P1 only. Counts connected 1-pixel components with 4-neighbor connectivity, or 8-neighbor connectivity with --eight. Prints <code>components N</code> and <code>sizes S1 S2 ...</code> in descending size order; an empty result prints just <code>sizes</code> on its second line.
- **ascii [--ramp CHARS]** — Any kind. Emits one character per pixel and LF per image row. P3 is converted to gray first; P1/P2 sample values are used directly. Default ramp is <code>@%#*+=-:. </code> (ten characters, including the final space). A custom ramp must contain 2–64 printable ASCII characters (space through tilde). For gray value v and ramp length L, index is <code>floor((v*(L-1)+floor(MAXVAL/2))/MAXVAL)</code>.

## Command-line and error behavior

Options can appear before, between, or after positional words. Words are processed left to right. Words not beginning with <code>-</code> (and the lone word <code>-</code>) are positional; an excess positional word is an error. An unrecognized option is an error, and repeating an option is an error. <code>--bins</code>, <code>--edge</code>, and <code>--ramp</code> consume the next word as their value even if it looks like an option. Missing/invalid values are rejected immediately. If all words are processed but positional arguments are missing, the command reports its required argument names.

Processing precedence is command word, command-line option/arity validation, complete image parsing, command image-kind validation, then semantic positional-argument validation and operation. Invalid command lines do not read standard input. The executable reads stdin only for a syntactically valid command invocation (including valid arity and option values), not for help/version or invalid invocations.

A command-line error exits 2; image parse errors, wrong image kinds, and image-dependent failures exit 1; success exits 0. Failure writes no stdout and exactly one LF-terminated diagnostic to stderr, prefixed with <code>pnmforge: </code>. The message catalog is:

| Condition | Message |
|---|---|
| No command | usage synopsis (to stderr) |
| Unknown command | <code>unknown command WORD</code> |
| Unknown option | <code>unknown option WORD</code> |
| Extra positional word | <code>unexpected argument WORD</code> |
| Repeated option | <code>WORD given twice</code> |
| Invalid/missing --bins value | <code>--bins needs an integer from 1 to 256</code> |
| Invalid/missing --edge value | <code>--edge needs one of clamp, zero, wrap</code> |
| Invalid/missing --ramp value | <code>--ramp needs 2 to 64 printable characters</code> |
| Missing positional arguments | <code>COMMAND needs NAMES</code> |
| No image token | <code>empty input</code> |
| Unsupported image magic | <code>unsupported format "TOKEN"</code> |
| Missing header fields | <code>truncated header</code> |
| Invalid dimensions | <code>bad width "TOKEN"</code> / <code>bad height "TOKEN"</code> |
| Invalid maxval | <code>bad maxval "TOKEN"</code> |
| Invalid P2/P3 sample | <code>bad sample "TOKEN"</code> |
| Invalid P1 sample character | <code>bad sample "TOKEN"</code> |
| Too few samples | <code>truncated data</code> |
| Additional token/bits | <code>trailing data</code> |
| histogram on P3 | <code>histogram needs a P1 or P2 image</code> |
| gray on P1 | <code>gray needs a P2 or P3 image</code> |
| threshold/levels on P1 | <code>threshold needs a P2 or P3 image</code> (or <code>levels needs a P2 or P3 image</code>) |
| flip argument | <code>flip needs h or v</code> |
| rotate argument | <code>rotate needs 90, 180 or 270</code> |
| crop arguments | <code>crop needs non-negative integers X Y and positive W H</code> |
| Crop outside image | <code>crop region outside the image</code> |
| Scale direction | <code>scale needs up or down</code> |
| Scale factor | <code>scale needs a factor from 2 to 16</code> |
| Scale-down result empty | <code>image too small to scale down by N</code> |
| Convolution kind/kernel | <code>convolve needs a P2 or P3 image</code> / <code>convolve needs blur, sharpen, edge or emboss</code> |
| Too many histogram bins | <code>too many bins for maxval MAXVAL</code> |
| Dither on P1 | <code>dither needs a P2 or P3 image</code> |
| Components on non-P1 | <code>components needs a P1 image</code> |
| Invalid threshold | <code>threshold needs an integer from 0 to 255</code> |
| Invalid levels range | <code>levels needs LOW below HIGH, both from 0 to 255</code> |
| Levels HIGH exceeds maxval | <code>levels HIGH above maxval MAXVAL</code> |
| Unexpected internal exception | <code>internal error</code> |

Where shown, WORD, TOKEN, COMMAND, NAMES, N, and MAXVAL are replaced by the offending word, token, command/argument names, factor, or image maxval. Parsing diagnostics preserve the offending token in double quotes.

## Worked examples

The following examples use POSIX shell syntax and the executable path for a Unix-like build. On Windows, use <code>build/Debug/pnmforge.exe</code> and a shell that supports equivalent stdin redirection (or provide the same PNM text through a file).

### Reports and canonical copy

~~~sh
printf 'P2 3 2 255 0 128 255 64 192 32\n' | build/pnmforge info
~~~

~~~text
format P2
size 3x2
maxval 255
samples 6
min 0
max 255
sum 671
~~~

Copying normalizes a commented image (this command's output is the canonical PNM text):

~~~sh
printf 'P2 # width follows\n2 2\n# maxval\n255 1 2\n3 4\n' | build/pnmforge copy
~~~

~~~text
P2
2 2
255
1 2
3 4
~~~

A histogram with four requested bins:

~~~sh
printf 'P2 3 2 255 0 128 255 64 192 32\n' | build/pnmforge histogram --bins 4
~~~

~~~text
0-63 2
64-127 1
128-191 1
192-255 2
~~~

### Tone

~~~sh
printf 'P2 3 2 255 0 128 255 64 192 32\n' | build/pnmforge invert
~~~

~~~text
P2
3 2
255
255 127 0
191 63 223
~~~

### Geometry

~~~sh
printf 'P2 2 3 9 1 2 3 4 5 6\n' | build/pnmforge rotate 90
~~~

~~~text
P2
3 2
9
5 3 1
6 4 2
~~~

### Scale

~~~sh
printf 'P2 2 3 9 1 2 3 4 5 6\n' | build/pnmforge scale up 2
~~~

~~~text
P2
4 6
9
1 1 2 2
1 1 2 2
3 3 4 4
3 3 4 4
5 5 6 6
5 5 6 6
~~~

### Convolution

~~~sh
printf 'P2 3 1 90 90 90 90\n' | build/pnmforge convolve --edge zero blur
~~~

~~~text
P2
3 1
90
20 30 20
~~~

### Dithering and connected components

~~~sh
printf 'P2 4 2 255 0 64 128 255 255 200 100 0\n' | build/pnmforge dither
~~~

~~~text
P1
4 2
1 1 0 0
0 0 1 1
~~~

A successful components example (4-connected) on a 4×3 bitmap:

~~~sh
printf 'P1 4 3 1 1 1 0 0 0 0 1 1 0 0 1\n' | build/pnmforge components
~~~

~~~text
components 3
sizes 3 2 1
~~~

### ASCII rendering

~~~sh
printf 'P2 5 1 255 0 64 128 192 255\n' | build/pnmforge ascii
~~~

~~~text
@#=: 
~~~

The trailing space on the ASCII output line is part of the output, followed by LF.

## Development

The CMake project builds the library, executable, and one test executable for each <code>tests/*_test.cpp</code> file. Tests use literal command-line rows through the public <code>run</code> and <code>reads_stdin</code> interfaces, comparing status and stdout/stderr exactly.

~~~sh
clang-format -i src/*.cpp src/*.hpp tests/*.cpp tests/*.hpp
clang-format --dry-run --Werror src/*.cpp src/*.hpp tests/*.cpp tests/*.hpp
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
~~~

The public interface and command table are in <code>src/pnmforge.hpp</code> and <code>src/registry.cpp</code>. Runtime argument parsing and stdin gating are in <code>src/cli.cpp</code>; parsing, canonical writing, and luminance are in <code>src/image.cpp</code>; each command family is implemented in its corresponding <code>src/cmd_*.cpp</code> module. The project enforces warnings as errors (<code>/W4 /WX</code> on MSVC and <code>-Wall -Wextra -Wpedantic -Werror</code> elsewhere). See [LICENSE](LICENSE) for the MIT license.

---

Built with [BAKED](https://github.com/Gulliby-development/baked), an open-source coding agent (bench round R83).
