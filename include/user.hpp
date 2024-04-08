#ifndef MANDELBROT_USER_HPP
#define MANDELBROT_USER_HPP

#include "error.h"

void create_window (const char *font_file_name, int font_size, int *code_error);

void draw_window (int *code_error);

#endif //MANDELBROT_USER_HPP
