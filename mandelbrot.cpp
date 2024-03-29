#include "mandelbrot.hpp"
#include "mandelbrot_avx.hpp"

static const int COLORS[9][2] = {{6, 10}, {20, 60}, {125, 235}, {97, 126}, {12, 60}, {234, 12}, {120, 90}, {20, 10}, {60, 30}};

void create_window (Window *window, const char *font_file_name, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        window->window_config.window.create(sf::VideoMode (WIDTH, HEIGHT), HEADING);
        window->window_config.image.create(WIDTH, HEIGHT);
        window->window_config.texture.create(WIDTH, HEIGHT);
        window->window_config.sprite.setTexture(window->window_config.texture);

        window->window_config.color[0] = COLORS[0][0];
        window->window_config.color[1] = COLORS[0][1];

        #ifdef FPS_ON
                if (!window->window_config.font_fps.loadFromFile(font_file_name))
                {
                        *code_error |= ERR_FOPEN;
                }

                window->window_config.text_fps.setFont(window->window_config.font_fps);
                window->window_config.text_fps.setCharacterSize(FONT_SIZE);

                window->window_config.text_fps.setFillColor(BLUE_COLOR);

        #endif

        window->window_position.x_offset = 0;
        window->window_position.y_offset = 0;
        window->window_position.zoom     = 1;
}

void draw_window (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        while (window->window_config.window.isOpen())
        {
                PROCESS_FPS (
                {
                        event_process(window, code_error);

                        frame_process(window, code_error);

                        window_update(&window->window_config, code_error);
                });

                ERR_RET ();
        }
}

inline void event_process (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        sf::Event event;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

        while (window->window_config.window.pollEvent(event))
        {
                switch (event.type)
                {
                        case (EventKeyPressed):
                        {
                                key_process(window, code_error);

                                break;
                        }
                        case (EventWindowClosed):
                        {
                                window->window_config.window.close();

                                break;
                        }
                        default:
                        {
                                break;
                        }
                }
        }

#pragma GCC diagnostic pop

}

inline void frame_process (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        if (window->window_config.use_avx)
        {
                process_mandelbrot_avx(window, code_error);
        }
        else
        {
                process_mandelbrot(window, code_error);
        }
}

inline void window_update (WindowConfig *window_config, int *code_error)
{
        my_assert(window_config != NULL, ERR_PTR);

        window_config->texture.update(window_config->image);

        window_config->window.clear();
        window_config->window.draw(window_config->sprite);

        #ifdef FPS_ON
                window_config->window.draw(window_config->text_fps);
        #endif

        window_config->window.display();
}

#define DEF_KEY(name, code)                                     \
        if (KeyPressed(name))                                   \
        {                                                       \
                code                                            \
        }                                                       \
        else

inline void key_process (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        #include "./includes/keys.h"
        {}
}

#undef DEF_KEY

inline void process_mandelbrot (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        for (int iy = 0; iy < HEIGHT; iy++)
        {
                for (int ix = 0; ix < WIDTH; ix++)
                {
                        double mandel_coord_x = (((double) ix + window->window_position.x_offset) / WIDTH  * COEF_SCALE_X - COEF_OFFSET_X) * window->window_position.zoom;
                        double mandel_coord_y = (((double) iy + window->window_position.y_offset) / HEIGHT * COEF_SCALE_Y - COEF_OFFSET_Y) * window->window_position.zoom;

                        volatile int belong_mandelbrot = check_point_mandelbrot(mandel_coord_x, mandel_coord_y);
                        ERR_RET();

                        set_color_pixel(&window->window_config, belong_mandelbrot, ix, iy, code_error);
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

void set_color_pixel (WindowConfig *window_config, const long long int belong_mandelbrot, const int ix, const int iy, int *code_error)
{
        my_assert(window_config != NULL, ERR_PTR);

        sf::Color color((belong_mandelbrot * window_config->color[0]) % N_MAX, 0, (belong_mandelbrot * window_config->color[1]) % N_MAX);
        window_config->image.setPixel(ix, iy, color);
}

#ifdef FPS_ON

void process_fps (WindowPerformance *window_performance, int *code_error)
{
        my_assert(window_performance != NULL, ERR_PTR);

        float time_sec = window_performance->clock_info.time.asSeconds();

        window_performance->fps = 1 / time_sec;
}

void print_fps (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        window->window_config.text_fps.setString(convert_float_to_str(window->window_performance.fps));
}

char *convert_float_to_str (float number)
{
        int number_len = snprintf(NULL, 0, "%.2f", number);

        char *result = (char *) calloc(number_len + 1, sizeof (char));
        snprintf(result, number_len + 1, "%.2f", number);

        return result;
}

#endif
