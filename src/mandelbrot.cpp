#ifndef MANDELBROT_CPP
#define MANDELBROT_CPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

#include "../includes/mandelbrot.hpp"
#include "../includes/mandelbrot_avx.hpp"
#include "../includes/mandelbrot_array.hpp"

#ifdef FPS_ON
        #define PROCESS_FRAME_PERFORMANCE(code)                                 \
                START_PROCESS_PERFORMANCE(window.window_perf.frame_time,        \
                                        window.window_perf.frame_ticks);        \
                code;                                                           \
                END_PROCESS_PERFORMANCE(window.window_perf.frame_time,          \
                                        window.window_perf.frame_ticks);        \

        #define PROCESS_RENDER_PERFORMANCE(code)                                \
                START_PROCESS_PERFORMANCE(window.window_perf.render_time,       \
                                        window.window_perf.render_ticks);       \
                code;                                                           \
                END_PROCESS_PERFORMANCE(window.window_perf.render_time,         \
                                        window.window_perf.render_ticks);       \

        #define START_PROCESS_PERFORMANCE(time, ticks)                          \
                time = window.window_perf.clock.getElapsedTime().asSeconds();   \
                ticks = __rdtsc();

        #define END_PROCESS_PERFORMANCE(time, ticks)                                    \
                time = window.window_perf.clock.getElapsedTime().asSeconds() - time;    \
                time = 1 / (time);                                                      \
                ticks = __rdtsc() - ticks;

#else
        #define PROCESS_FRAME_PERFORMANCE(code)  code
        #define PROCESS_RENDER_PERFORMANCE(code) code
#endif

static const char *HEADING = "Mandelbrot!";

static const int  FONT_SIZE = 24;

#ifdef FPS_ON
        static const char *FRAME_STR  = "Frame time / Frame ticks";
        static const char *RENDER_STR = "Render time / Render ticks";

        static const int LEN_STR_FPS = 100;
#endif

static const double COEF_WINDOW_ZOOM = 1.1;
static const int    COEF_WINDOW_MOVE = 10;

static const int N_COLORS = 9;
static const int N_PARAMS = 2;

static const int COLORS[N_COLORS][N_PARAMS] = {{6, 10}, {20, 60}, {125, 235}, {97, 126}, {12, 60}, {234, 12}, {120, 90}, {20, 10}, {60, 30}};

static Window window = {};

static inline void event_process (struct Window *window, int *code_error);
static inline void frame_process (struct Window *window, int *code_error);
static inline void window_update (WindowConfig *window_config, int *code_error);
static inline void key_process (struct Window *window, int *code_error);
static inline void mouse_wheel_process (struct Window *window, sf::Event *event, int *code_error);
static inline void process_mandelbrot (struct Window *window, int *code_error);
static inline int check_point_mandelbrot (const double start_coord_x, const double start_coord_y);

#ifdef FPS_ON
        static void set_string_fps (struct Window *window, int *code_error);
#endif

void create_window (const char *font_file_name, int *code_error)
{
        my_assert (font_file_name != NULL, ERR_PTR);

        window.window_config.window.create(sf::VideoMode (WIDTH, HEIGHT), HEADING);
        window.window_config.image.create(WIDTH, HEIGHT);
        window.window_config.texture.create(WIDTH, HEIGHT);
        window.window_config.sprite.setTexture(window.window_config.texture);

        window.window_config.n_color = 0;

        window.window_config.use_avx   = false;
        window.window_config.use_array = false;

        #ifdef FPS_ON
                if (!window.window_config.font_fps.loadFromFile(font_file_name))
                {
                        *code_error |= ERR_FOPEN;
                }

                window.window_config.text_fps.setFont(window.window_config.font_fps);
                window.window_config.text_fps.setCharacterSize(FONT_SIZE);

                window.window_config.text_fps.setFillColor(BLUE_COLOR);

                window.window_perf.frame_ticks  = 0;
                window.window_perf.frame_time   = 0;
                window.window_perf.render_ticks = 0;
                window.window_perf.render_time  = 0;

        #endif

        window.window_position.x_offset = 0;
        window.window_position.y_offset = 0;
        window.window_position.zoom     = 1;
}

void draw_window (int *code_error)
{
        int n_color = 0;

        while (window.window_config.window.isOpen())
        {
                n_color++;

                PROCESS_FRAME_PERFORMANCE(
                {
                        event_process(&window, code_error);
                        ERR_RET();

                        PROCESS_RENDER_PERFORMANCE(frame_process(&window, code_error));
                        ERR_RET();

                        window_update(&window.window_config, code_error);
                        ERR_RET();
                })

                #ifdef FPS_ON
                        set_string_fps(&window, code_error);
                        window_update(&window.window_config, code_error);
                #endif

                if (n_color == 4)
                {
                        window.window_config.n_color += 1;

                        if (window.window_config.n_color == 9)
                        {
                                window.window_config.n_color = 0;
                        }

                        n_color = 0;
                }
        }
}

inline void event_process (struct Window *window, int *code_error)
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
                        case (EventWheelScrolled):
                        {
                                mouse_wheel_process(window, &event, code_error);

                                break;
                        }
                        default:
                        {
                                break;
                        }
                }

                ERR_RET();
        }

#pragma GCC diagnostic pop

}

inline void frame_process (struct Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        if (window->window_config.use_avx)
        {
                process_mandelbrot_avx(window, code_error);
        }
        else if (window->window_config.use_array)
        {
                process_mandelbrot_array(window, code_error);
        }
        else
        {
                process_mandelbrot(window, code_error);
        }

        ERR_RET();
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

inline void key_process (struct Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        #include "..//includes/keys.h"
        {}
}

#undef DEF_KEY

inline void mouse_wheel_process (struct Window *window, sf::Event *event, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);
        my_assert(event  != NULL, ERR_PTR);

        if (event->mouseWheelScroll.delta > 0)
        {
                if (window->window_config.n_color == 8)
                {
                        window->window_config.n_color = 0;
                }
                else
                {
                        window->window_config.n_color++;
                }
        }
        else
        {
                if (window->window_config.n_color == 0)
                {
                        window->window_config.n_color = 8;
                }
                else
                {
                        window->window_config.n_color--;
                }
        }
}

inline void process_mandelbrot (struct Window *window, int *code_error)
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

void set_color_pixel (WindowConfig *window_config, const long long int belong_mandelbrot, const int ix, const int iy, int *code_error)
{
        my_assert(window_config != NULL, ERR_PTR);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

        sf::Color color((belong_mandelbrot * COLORS[window_config->n_color][0]) % N_MAX, 0, (belong_mandelbrot * COLORS[window_config->n_color][1]) % N_MAX);
        window_config->image.setPixel(ix, iy, color);

#pragma GCC diagnostic pop

}

#ifdef FPS_ON

void set_string_fps (struct Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        char *str_fps = (char *) calloc(LEN_STR_FPS, sizeof(char));

        sprintf(str_fps, "%s : %.2f / %ld\n\r%s : %.2f / %ld\n\r",
                FRAME_STR, window->window_perf.frame_time, window->window_perf.frame_ticks,
                RENDER_STR, window->window_perf.render_time, window->window_perf.render_ticks);

        window->window_config.text_fps.setString(str_fps);
}

#endif

#pragma GCC diagnostic pop

#endif //MANDELBROT_CPP
