#ifndef MANDELBROT_AVX_HPP
#define MANDELBROT_AVX_HPP

#include <immintrin.h>

#include "render_frame.hpp"

void process_mandelbrot_avx (Window *window, int *code_error);

#endif // MANDELBROT_AVX_HPP
