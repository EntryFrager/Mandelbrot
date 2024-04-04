#ifndef MANDELBROT_ARRAY_CPP
#define MANDELBROT_ARRAY_CPP

#include "../include/mandelbrot_array.hpp"

static inline void check_point_mandelbrot_array (const double *start_coord_x, const double *start_coord_y, long long int *belong_mandelbrot_array, int *code_error);
static inline void set_color_pixel_array (WindowConfig *window_config, const long long int *belong_mandelbrot_array, const int ix, const int iy, int *code_error);

void process_mandelbrot_array (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        const double scale_x = WIDTH  / (2 * window->window_position.zoom) + window->window_position.x_offset;
        const double scale_y = HEIGHT / (2 * window->window_position.zoom) + window->window_position.y_offset;

        for (int iy = 0; iy < HEIGHT; iy++)
        {
                double mandel_coord_y[VECTOR_SIZE] = {(double) iy / window->window_position.zoom - scale_y,
                                                      (double) iy / window->window_position.zoom - scale_y,
                                                      (double) iy / window->window_position.zoom - scale_y,
                                                      (double) iy / window->window_position.zoom - scale_y};

                for (int ix = 0; ix < WIDTH; ix += 4)
                {
                        double mandel_coord_x[VECTOR_SIZE] = {((double) ix    ) / window->window_position.zoom - scale_x,
                                                              ((double) ix + 1) / window->window_position.zoom - scale_x,
                                                              ((double) ix + 2) / window->window_position.zoom - scale_x,
                                                              ((double) ix + 3) / window->window_position.zoom - scale_x};

                        long long int belong_mandelbrot_array[VECTOR_SIZE] = {0};

                        check_point_mandelbrot_array(mandel_coord_x, mandel_coord_y, belong_mandelbrot_array, code_error);
                        ERR_RET();

                        set_color_pixel_array(&window->window_config, belong_mandelbrot_array, ix, iy, code_error);
                }
        }
}

inline void check_point_mandelbrot_array (const double *start_coord_x, const double *start_coord_y, long long int *belong_mandelbrot_array, int *code_error)
{
    my_assert(start_coord_x           != NULL, ERR_PTR);
    my_assert(start_coord_y           != NULL, ERR_PTR);
    my_assert(belong_mandelbrot_array != NULL, ERR_PTR);

    double x[VECTOR_SIZE] = {};
    double y[VECTOR_SIZE] = {};

    for (int n_point = 0; n_point < VECTOR_SIZE; n_point++)
        {
            x[n_point] = start_coord_x[n_point];
            y[n_point] = start_coord_y[n_point];
        }

    double x_square[VECTOR_SIZE] = {};
    double y_square[VECTOR_SIZE] = {};
    double prod_x_y[VECTOR_SIZE] = {};

    for (int n_iterate = 1; n_iterate < N_MAX; n_iterate++)
    {
        for (int n_point = 0; n_point < VECTOR_SIZE; n_point++)
        {
            x_square[n_point] = x[n_point] * x[n_point];
            y_square[n_point] = y[n_point] * y[n_point];
            prod_x_y[n_point] = x[n_point] * y[n_point];

            if (x_square[n_point] + y_square[n_point] > MAX_RADIUS && belong_mandelbrot_array[n_point] == 0)
            {
                belong_mandelbrot_array[n_point] = n_iterate;
            }

            x[n_point] = x_square[n_point] - y_square[n_point] + start_coord_x[n_point];
            y[n_point] = 2 * prod_x_y[n_point]           + start_coord_y[n_point];
        }
    }
}

inline void set_color_pixel_array (WindowConfig *window_config, const long long int *belong_mandelbrot_array, const int ix, const int iy, int *code_error)
{
    my_assert(window_config           != NULL, ERR_PTR);
    my_assert(belong_mandelbrot_array != NULL, ERR_PTR);

    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        set_color_pixel(window_config, belong_mandelbrot_array[i], ix + i, iy, code_error);
    }
}

#endif // MANDELBROT_ARRAY_CPP
