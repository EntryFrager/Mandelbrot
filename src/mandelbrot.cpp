#ifndef MANDELBROT_CPP
#define MANDELBROT_CPP

#include "../include/mandelbrot.hpp"

static inline int check_point_mandelbrot (const double start_coord_x, const double start_coord_y);

void process_mandelbrot (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        const double scale_x = WIDTH  / (2 * window->window_position.zoom) + window->window_position.x_offset;
        const double scale_y = HEIGHT / (2 * window->window_position.zoom) + window->window_position.y_offset;

        for (int iy = 0; iy < HEIGHT; iy++)
        {
                double mandel_coord_y = (double) iy / window->window_position.zoom - scale_y;

                for (int ix = 0; ix < WIDTH; ix++)
                {
                        double mandel_coord_x = (double) ix / window->window_position.zoom - scale_x;

                        volatile int belong_mandelbrot = check_point_mandelbrot(mandel_coord_x, mandel_coord_y);
                        ERR_RET();

                        set_color_pixel(&window->window_config, belong_mandelbrot, ix, iy, code_error);
                        ERR_RET();
                }
        }
}

inline int check_point_mandelbrot (const double start_coord_x, const double start_coord_y)
{
    double x = start_coord_x;
    double y = start_coord_y;

    for (int i = 1; i < N_MAX; i++)
    {
        double x_square = x * x;
        double y_square = y * y;
        double prod_x_y = x * y;

        if ((x_square + y_square) > MAX_RADIUS)
        {
                return i;
        }

        x = x_square - y_square + start_coord_x;
        y = 2 * prod_x_y        + start_coord_y;
    }

    return 0;
}

#endif // MANDELBROT_CPP
