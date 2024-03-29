#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "./includes/dsl.hpp"
#include "./includes/error.h"

#define FPS_ON

#ifdef FPS_ON
    #define PROCESS_FPS(code)                                                                                           \
    {                                                                                                                   \
        window->window_performance.clock_info.clock.restart ();                                                         \
        code                                                                                                            \
        window->window_performance.clock_info.time = window->window_performance.clock_info.clock.getElapsedTime ();     \
        process_fps(&window->window_performance, code_error);                                                           \
        print_fps(window, code_error);                                                                                  \
        window_update(&window->window_config, code_error);                                                              \
    }
#else
    #define PROCESS_FPS(code) code
#endif

const int HEIGHT = 600;
const int WIDTH  = 800;

const char HEADING[] = "Mandelbrot!";

const char FONT_FILE_NAME[] = "./includes/arialmt.ttf";
const int FONT_SIZE = 24;

const double MAX_RADIUS = 10;

const int N_MAX = 256;

const double COEF_WINDOW_ZOOM = 1.1;
const int    COEF_WINDOW_MOVE = 10;

const double COEF_SCALE_X  = 3.0f;
const double COEF_SCALE_Y  = 2.0f;
const double COEF_OFFSET_X = 2.0f;
const double COEF_OFFSET_Y = 1.0f;

#ifdef FPS_ON
    typedef struct {
        sf::Time time;
        sf::Clock clock;
    } ClockInfo;

    typedef struct {
        float fps = 0;
        ClockInfo clock_info = {};
    } WindowPerformance;
#endif

typedef struct {
    sf::RenderWindow window;

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    #ifdef FPS_ON
        sf::Font font_fps;
        sf::Text text_fps;
    #endif

    bool use_avx = false;

    int color[2] = {};
} WindowConfig;

typedef struct {
    double x_offset = 0;
    double y_offset = 0;
    double zoom     = 1;
} WindowPosition;

typedef struct {
    WindowConfig window_config = {};

    WindowPerformance window_performance = {};

    WindowPosition window_position = {};
} Window;

void create_window (Window *window, const char *font_file_name, int *code_error);

void draw_window (Window *window, int *code_error);

inline void event_process (Window *window, int *code_error);

inline void frame_process (Window *window, int *code_error);

inline void window_update (WindowConfig *window_config, int *code_error);

inline void key_process (Window *window, int *code_error);

inline void process_mandelbrot (Window *window, int *code_error);

inline int check_point_mandelbrot (const double start_coord_x, const double start_coord_y);

void set_color_pixel (WindowConfig *window_config, const long long int belong_mandelbrot, const int ix, const int iy, int *code_error);

#ifdef FPS_ON
    void process_fps (WindowPerformance *window_performance, int *code_error);

    void print_fps (Window *window, int *code_error);

    char *convert_float_to_str (float number);
#endif

#endif //MANDELBROT_HPP
