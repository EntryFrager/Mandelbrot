#include "mandelbrot.hpp"
#include "mandelbrot_avx.hpp"

void draw_mandelbrot_avx (WindowSet *window_set, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        __m256d VECTOR_START_CARRY_X  = _mm256_set_pd (3, 2, 1, 0);
        __m256d VECTOR_X_OFFSET       = _mm256_set1_pd (X_OFFSET);
        __m256d VECTOR_Y_OFFSET       = _mm256_set1_pd (Y_OFFSET);
        __m256d VECTOR_WIDTH          = _mm256_set1_pd ((double) WIDTH);
        __m256d VECTOR_HEIGHT         = _mm256_set1_pd ((double) HEIGHT);
        __m256d VECTOR_SCOPE          = _mm256_set1_pd (SCOPE);
        __m256d VECTOR_COEF_STRETCH_X = _mm256_set1_pd (COEF_STRETCH_X);
        __m256d VECTOR_COEF_STRETCH_Y = _mm256_set1_pd (COEF_STRETCH_Y);
        __m256d VECTOR_COEF_CARRY_X   = _mm256_set1_pd (COEF_CARRY_X);
        __m256d VECTOR_COEF_CARRY_Y   = _mm256_set1_pd (COEF_CARRY_Y);

        for (int iy = 0; iy < HEIGHT; iy++)
        {
                for (int ix = 0; ix < WIDTH; ix += 4)
                {
                        __m256d mandel_coord_x = _mm256_add_pd (_mm256_set_pd ((double) ix, (double) ix, (double) ix, (double) ix), VECTOR_START_CARRY_X);

                        mandel_coord_x = _mm256_div_pd (_mm256_add_pd (mandel_coord_x, VECTOR_X_OFFSET), VECTOR_WIDTH);
                        mandel_coord_x = _mm256_sub_pd (_mm256_mul_pd (mandel_coord_x, VECTOR_COEF_STRETCH_X), VECTOR_COEF_CARRY_X);
                        mandel_coord_x = _mm256_mul_pd (mandel_coord_x, VECTOR_SCOPE);

                        __m256d mandel_coord_y = _mm256_set1_pd ((double) iy);

                        mandel_coord_y = _mm256_div_pd (_mm256_add_pd (mandel_coord_y, VECTOR_Y_OFFSET), VECTOR_HEIGHT);
                        mandel_coord_y = _mm256_sub_pd (_mm256_mul_pd (mandel_coord_y, VECTOR_COEF_STRETCH_Y), VECTOR_COEF_CARRY_Y);
                        mandel_coord_y = _mm256_mul_pd (mandel_coord_y, VECTOR_SCOPE);

                        volatile __m256i belong_mandelbrot = _mm256_setzero_si256 ();

                        check_point_mandelbrot_avx (mandel_coord_x, mandel_coord_y, &belong_mandelbrot, code_error);
                        ERR_RET ();

                        draw_vector_color (window_set, (int *) &belong_mandelbrot, ix, iy, code_error);
                }
        }
}

inline void check_point_mandelbrot_avx (const __m256d start_coord_x, const __m256d start_coord_y, volatile __m256i *belong_mandelbrot, int *code_error)
{
        my_assert (belong_mandelbrot != NULL, ERR_PTR);

        __m256d x = start_coord_x;
        __m256d y = start_coord_y;

        for (int i = 1; i < N_MAX; i++)
        {
                __m256d x_square = _mm256_mul_pd (x, x);
                __m256d y_square = _mm256_mul_pd (y, y);
                __m256d prod_x_y = _mm256_mul_pd (x, y);

                __m256d R_x_y = _mm256_add_pd (x_square, y_square);

                __m256d cmp = _mm256_cmp_pd (R_x_y, VECTOR_MAX_RADIUS, _CMP_LE_OQ);

                int mask = _mm256_movemask_pd (cmp);

                if (!mask)
                {
                        break;
                }

                *belong_mandelbrot = _mm256_sub_epi64 (*belong_mandelbrot, _mm256_castpd_si256 (cmp));

                x = _mm256_add_pd (_mm256_sub_pd (x_square, y_square), start_coord_x);
                y = _mm256_add_pd (_mm256_add_pd (prod_x_y, prod_x_y), start_coord_y);
        }
}

inline void draw_vector_color (WindowSet *window_set, const int *belong_mandelbrot, const int ix, const int iy, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        for (int i = 0; i < 4; i++)
        {
                sf::Color color ((belong_mandelbrot[i] * 6) % 256, 0, (belong_mandelbrot[i] * 10) % 256);
                IMAGE.setPixel (ix + i, iy, color);
        }
}
