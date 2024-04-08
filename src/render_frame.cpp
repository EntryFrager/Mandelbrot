#ifndef RENDER_FRAME_CPP
#define RENDER_FRAME_CPP

#include "../include/render_frame.hpp"

#include "../include/mandelbrot.hpp"
#include "../include/mandelbrot_array.hpp"
#include "../include/mandelbrot_avx.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

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

#ifdef FPS_ON
        static const char *FRAME_STR  = "Frame time / Frame ticks";
        static const char *RENDER_STR = "Render time / Render ticks";

        static const int LEN_STR_FPS = 100;
#endif

static const double COEF_WINDOW_ZOOM = 1.1;
static const double COEF_WINDOW_MOVE = 10;

static const int N_COLORS = 9;

static const COLOR COLORS[N_COLORS] = {{6, 0, 10}, {20, 0, 60}, {125, 0, 235}, {97, 0, 126}, {12, 0, 60}, {234, 0, 12}, {120, 0, 90}, {20, 0, 10}, {60, 0, 30}};

static Window window = {};

static void event_process (Window *window, int *code_error);
static inline void frame_process (Window *window, int *code_error);
static inline void window_update (WindowConfig *window_config, int *code_error);
static void key_process (Window *window, int *code_error);
static inline void mouse_wheel_process (Window *window, sf::Event *event, int *code_error);
static inline void switch_color (WindowConfig *window_config, int *code_error);

#ifdef FPS_ON
        static void set_string_fps (Window *window, int *code_error);
#endif

void create_window (const char *font_file_name, int font_size, int *code_error)
{
        my_assert (font_file_name != NULL, ERR_PTR);

        if (window.window_is_create)
        {
                return;
        }

        window.window_is_create = true;

        window.window_config.window.create(sf::VideoMode (WIDTH, HEIGHT), HEADING);
        window.window_config.image.create(WIDTH, HEIGHT);
        window.window_config.texture.create(WIDTH, HEIGHT);
        window.window_config.sprite.setTexture(window.window_config.texture);

        window.window_config.n_color = 0;

        window.window_config.auto_switch_color = false;

        window.window_config.use_avx   = false;
        window.window_config.use_array = false;

        #ifdef FPS_ON
                if (!window.window_config.font_fps.loadFromFile(font_file_name))
                {
                        *code_error |= ERR_FOPEN;
                }

                window.window_config.text_fps.setFont(window.window_config.font_fps);
                window.window_config.text_fps.setCharacterSize(font_size);

                window.window_config.text_fps.setFillColor(BLUE_COLOR);

                window.window_perf.frame_ticks  = 0;
                window.window_perf.frame_time   = 0;
                window.window_perf.render_ticks = 0;
                window.window_perf.render_time  = 0;

        #endif

        window.window_position.x_offset = 0.6;
        window.window_position.y_offset = 0;
        window.window_position.zoom     = 280;
}

void draw_window (int *code_error)
{
        while (window.window_config.window.isOpen())
        {
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

                if (window.window_config.auto_switch_color)
                {
                        switch_color(&window.window_config, code_error);
                }
        }
}

void event_process (Window *window, int *code_error)
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

inline void frame_process (Window *window, int *code_error)
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

void key_process (Window *window, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);

        #include "../include/keys.h"
        {}
}

#undef DEF_KEY

inline void mouse_wheel_process (Window *window, sf::Event *event, int *code_error)
{
        my_assert(window != NULL, ERR_PTR);
        my_assert(event  != NULL, ERR_PTR);

        if (event->mouseWheelScroll.delta > 0)
        {
                window->window_config.n_color++;

                if (window->window_config.n_color == N_COLORS)
                {
                        window->window_config.n_color = 0;
                }
        }
        else
        {
                window->window_config.n_color--;

                if (window->window_config.n_color == -1)
                {
                        window->window_config.n_color = N_COLORS - 1;
                }
        }
}

void set_color_pixel (WindowConfig *window_config, const long long int belong_mandelbrot, const int ix, const int iy, int *code_error)
{
        my_assert(window_config != NULL, ERR_PTR);

        sf::Color color(((unsigned char) (belong_mandelbrot * COLORS[window_config->n_color].color_r) % N_MAX),
                         (unsigned char) COLORS[window_config->n_color].color_g,
                         (unsigned char) ((belong_mandelbrot * COLORS[window_config->n_color].color_b) % N_MAX));
        window_config->image.setPixel(ix, iy, color);
}

inline void switch_color (WindowConfig *window_config, int *code_error)
{
        my_assert (window_config != NULL, ERR_PTR);

        static int n_frame = 0;

        n_frame++;

        if (n_frame == 5)
        {
                window_config->n_color += 1;

                if (window_config->n_color == 9)
                {
                        window_config->n_color = 0;
                }

                n_frame = 0;
        }
}

#ifdef FPS_ON

void set_string_fps (Window *window, int *code_error)
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

#endif // RENDER_FRAME_CPP
