#include "..//includes/mandelbrot_array.hpp"

static inline void check_point_mandelbrot_array (const double *start_coord_x, const double *start_coord_y, long long int *belong_mandelbrot_array, int *code_error);
static inline void set_color_pixel_array (WindowConfig *window_config, const long long int *belong_mandelbrot_array, const int ix, const int iy, int *code_error);

void process_mandelbrot_array (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        for (int iy = 0; iy < HEIGHT; iy++)
        {
                for (int ix = 0; ix < WIDTH; ix += 4)
                {
                        double mandel_coord_x[VECTOR_SIZE] = {(((double) ix     + window->window_position.x_offset) / WIDTH  * COEF_SCALE_X - COEF_OFFSET_X) * window->window_position.zoom,
                                                              (((double) ix + 1 + window->window_position.x_offset) / WIDTH  * COEF_SCALE_X - COEF_OFFSET_X) * window->window_position.zoom,
                                                              (((double) ix + 2 + window->window_position.x_offset) / WIDTH  * COEF_SCALE_X - COEF_OFFSET_X) * window->window_position.zoom,
                                                              (((double) ix + 3 + window->window_position.x_offset) / WIDTH  * COEF_SCALE_X - COEF_OFFSET_X) * window->window_position.zoom};

                        double mandel_coord_y[VECTOR_SIZE] = {(((double) iy     + window->window_position.y_offset) / HEIGHT * COEF_SCALE_Y - COEF_OFFSET_Y) * window->window_position.zoom,
                                                              (((double) iy     + window->window_position.y_offset) / HEIGHT * COEF_SCALE_Y - COEF_OFFSET_Y) * window->window_position.zoom,
                                                              (((double) iy     + window->window_position.y_offset) / HEIGHT * COEF_SCALE_Y - COEF_OFFSET_Y) * window->window_position.zoom,
                                                              (((double) iy     + window->window_position.y_offset) / HEIGHT * COEF_SCALE_Y - COEF_OFFSET_Y) * window->window_position.zoom};

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
