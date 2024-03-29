#ifndef MANDELBROT_AVX__HPP
#define MANDELBROT_AVX_HPP

const __m256d VECTOR_MAX_RADIUS = _mm256_set1_pd (MAX_RADIUS);

void draw_mandelbrot_avx (WindowSet *window_set, int *code_error);

inline void check_point_mandelbrot_avx (const __m256d start_coord_x, const __m256d start_coord_y, volatile __m256i *belong_mandelbrot, int *code_error);

inline void draw_vector_color (WindowSet *window_set, const long long int *belong_mandelbrot, const int ix, const int iy, int *code_error);

#endif //MANDELBROT_AVX_HPP
