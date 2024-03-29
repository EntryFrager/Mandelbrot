#ifndef MANDELBROT_AVX__HPP
#define MANDELBROT_AVX_HPP

void process_mandelbrot_avx (Window *window, int *code_error);

inline void check_point_mandelbrot_avx (const __m256d start_coord_x, const __m256d start_coord_y, volatile __m256i *belong_mandelbrot, int *code_error);

inline void set_vector_color (WindowConfig *window_config, const long long int *belong_mandelbrot, const int ix, const int iy, int *code_error);

#endif //MANDELBROT_AVX_HPP
