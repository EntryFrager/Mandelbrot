#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "dsl.hpp"
#include "error.h"

#define FPS_ON

#ifdef FPS_ON
    #define PROCESS_FRAME_PERFORMANCE(code)                                                                         \
        window->window_performance.clock.restart();                                                                 \
        window->window_performance.frame_ticks = (-1) * __rdtsc();                                                  \
        code;                                                                                                       \
        window->window_performance.frame_time  = 1 / window->window_performance.clock.getElapsedTime().asSeconds(); \
        window->window_performance.frame_ticks += __rdtsc();                                                        \
        set_string_fps(window, code_error);                                                                         \
        window_update(&window->window_config, code_error);

    #define PROCESS_RENDER_PERFORMANCE(code)                                                                            \
        window->window_performance.render_time = (-1) * window->window_performance.clock.getElapsedTime().asSeconds();  \
        window->window_performance.render_ticks = (-1) * __rdtsc();                                                     \
        code;                                                                                                           \
        window->window_performance.render_time  += window->window_performance.clock.getElapsedTime().asSeconds();       \
        window->window_performance.render_time   = 1 / window->window_performance.render_time;                          \
        window->window_performance.render_ticks += __rdtsc();
#else
    #define PROCESS_FRAME_PERFORMANCE(code)  code
    #define PROCESS_RENDER_PERFORMANCE(code) code
#endif

const int HEIGHT = 600;
const int WIDTH  = 800;

const char HEADING[] = "Mandelbrot!";

const char FONT_FILE_NAME[] = "./includes/arialmt.ttf";
const int  FONT_SIZE = 24;

#ifdef FPS_ON
    const char FRAME_TIME_STR[]  = "Frame time";
    const char RENDER_TIME_STR[] = "Render time";

    const char FRAME_TICKS_STR[]  = "Frame ticks";
    const char RENDER_TICKS_STR[] = "Render ticks";

    const int LEN_STR_FPS = 100;
#endif

const double MAX_RADIUS = 10;

const int N_MAX = 256;

const int VECTOR_SIZE = 4;

const double COEF_WINDOW_ZOOM = 1.1;
const int    COEF_WINDOW_MOVE = 10;

const double COEF_SCALE_X  = 3.0f;
const double COEF_SCALE_Y  = 2.0f;
const double COEF_OFFSET_X = 2.0f;
const double COEF_OFFSET_Y = 1.0f;

#ifdef FPS_ON
    typedef struct {
        float     frame_time   = 0;
        float     render_time  = 0;

        uint64_t  frame_ticks  = 0;
        uint64_t  render_ticks = 0;

        sf::Clock clock;
    } WindowPerformance;
#endif

typedef struct {
    sf::RenderWindow window;

    sf::Image   image;
    sf::Texture texture;
    sf::Sprite  sprite;

    #ifdef FPS_ON
        sf::Font font_fps;
        sf::Text text_fps;
    #endif

    bool use_avx   = false;
    bool use_array = false;

    int n_color = 0;
} WindowConfig;

typedef struct {
    double x_offset = 0;
    double y_offset = 0;
    double zoom     = 1;
} WindowPosition;

typedef struct {
    WindowConfig window_config = {};

    #ifdef FPS_ON
        WindowPerformance window_performance = {};
    #endif

    WindowPosition window_position = {};
} Window;

void create_window (Window *window, const char *font_file_name, int *code_error);

void draw_window (Window *window, int *code_error);

void set_color_pixel (WindowConfig *window_config, const long long int belong_mandelbrot, const int ix, const int iy, int *code_error);

#endif //MANDELBROT_HPP
