#ifndef MANDELBROT_AVX_CPP
#define MANDELBROT_AVX_CPP

#include "../includes/mandelbrot_avx.hpp"

static const __m256d VECTOR_MAX_RADIUS = _mm256_set1_pd (MAX_RADIUS);

static inline void check_point_mandelbrot_avx (const __m256d start_coord_x, const __m256d start_coord_y, volatile __m256i *belong_mandelbrot_avx, int *code_error);
static inline void set_vector_color (WindowConfig *window_config, const long long int *belong_mandelbrot_avx, const int ix, const int iy, int *code_error);

void process_mandelbrot_avx (struct Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        __m256d VECTOR_START_CARRY_X  = _mm256_set_pd((double) 3 / WIDTH * COEF_SCALE_X * window->window_position.zoom,
                                                      (double) 2 / WIDTH * COEF_SCALE_X * window->window_position.zoom,
                                                      (double) 1 / WIDTH * COEF_SCALE_X * window->window_position.zoom,
                                                      (double) 0);

        for (int iy = 0; iy < HEIGHT; iy++)
        {
                for (int ix = 0; ix < WIDTH; ix += 4)
                {
                        __m256d mandel_coord_x = _mm256_set1_pd((((double) ix + window->window_position.x_offset) / WIDTH  * COEF_SCALE_X - COEF_OFFSET_X) * window->window_position.zoom);

                        mandel_coord_x = _mm256_add_pd (mandel_coord_x, VECTOR_START_CARRY_X);

                        __m256d mandel_coord_y = _mm256_set1_pd((((double) iy + window->window_position.y_offset) / HEIGHT * COEF_SCALE_Y - COEF_OFFSET_Y) * window->window_position.zoom);

                        volatile __m256i belong_mandelbrot_avx = _mm256_set1_epi64x(N_MAX - 1);

                        check_point_mandelbrot_avx(mandel_coord_x, mandel_coord_y, &belong_mandelbrot_avx, code_error);
                        ERR_RET();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

                        set_vector_color(&window->window_config, (long long int *) &belong_mandelbrot_avx, ix, iy, code_error);

#pragma GCC diagnostic pop
                }
        }
}

inline void check_point_mandelbrot_avx (const __m256d start_coord_x, const __m256d start_coord_y, volatile __m256i *belong_mandelbrot_avx, int *code_error)
{
        my_assert(belong_mandelbrot_avx != NULL, ERR_PTR);

        __m256d x = start_coord_x;
        __m256d y = start_coord_y;

        int mask = 0;

        for (int i = 1; i < N_MAX; i++)
        {
                __m256d x_square = _mm256_mul_pd(x, x);
                __m256d y_square = _mm256_mul_pd(y, y);
                __m256d prod_x_y = _mm256_mul_pd(x, y);

                __m256d R_x_y = _mm256_add_pd(x_square, y_square);

                __m256d cmp = _mm256_cmp_pd(R_x_y, VECTOR_MAX_RADIUS, _CMP_LE_OQ);

                mask = _mm256_movemask_pd(cmp);

                if (!mask)
                {
                        break;
                }

                *belong_mandelbrot_avx = _mm256_add_epi64(*belong_mandelbrot_avx, _mm256_castpd_si256 (cmp));

                x = _mm256_add_pd(_mm256_sub_pd(x_square, y_square), start_coord_x);
                y = _mm256_add_pd(_mm256_add_pd(prod_x_y, prod_x_y), start_coord_y);
        }

        __m256i reverse_sign = _mm256_set1_epi64x(-1);

        *belong_mandelbrot_avx = _mm256_mul_epi32(*belong_mandelbrot_avx, reverse_sign);
}

inline void set_vector_color (WindowConfig *window_config, const long long int *belong_mandelbrot_avx, const int ix, const int iy, int *code_error)
{
        my_assert(window_config != NULL, ERR_PTR);
        my_assert(belong_mandelbrot_avx != NULL, ERR_PTR);

        for (int i = 0; i < VECTOR_SIZE; i++)
        {
                set_color_pixel(window_config, belong_mandelbrot_avx[i], ix + i, iy, code_error);
        }
}

#endif //MANDELBROT_AVX_CPP
