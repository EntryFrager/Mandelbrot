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
    #define COUNT_FPS(code)                     \
    {                                           \
        CLOCK.restart ();                       \
        code                                    \
        TIME = CLOCK.getElapsedTime ();         \
        if (FLAG_OFFSET_WINDOW)                 \
        {                                       \
            count_fps (window_set, code_error); \
            print_fps (window_set, code_error); \
        }                                       \
    }
#else
    #define COUNT_FPS(code) code
#endif

const int HEIGHT = 600;
const int WIDTH  = 800;

const char HEADING[] = "Mandelbrot!";

const int CHARACTER_SIZE = 24;

const double MAX_RADIUS = 10;

const int N_MAX = 256;

const double COEF_WINDOW_MOVE_U_D = 1.1;
const int    COEF_WINDOW_MOVE_R_L = 10;

const double COEF_STRETCH_X = 3.0f;
const double COEF_STRETCH_Y = 2.0f;
const double COEF_CARRY_X   = 2.0f;
const double COEF_CARRY_Y   = 1.0f;

#ifdef FPS_ON
    typedef struct {
        sf::Time time;
        sf::Clock clock;
    } ClockInfo;
#endif

typedef struct {
    sf::RenderWindow window;

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    bool use_avx = false;

    #ifdef FPS_ON
        sf::Font font_fps;
        sf::Text text_fps;

        float fps = 0;

        ClockInfo clock_info = {};
    #endif

    bool flag_offset_window = true;

    double x_offset = 0;
    double y_offset = 0;
    double scope    = 1;
} WindowSet;

void create_window (WindowSet *window_set, const char *font_file_name, int *code_error);

void draw_window (WindowSet *window_set, int *code_error);

void event_process (WindowSet *window_set, int *code_error);

void draw_mandelbrot (WindowSet *window_set, int *code_error);

inline int check_point_mandelbrot (const double start_coord_x, const double start_coord_y);

#ifdef FPS_ON
    void count_fps (WindowSet *window_set, int *code_error);

    void print_fps (WindowSet *window_set, int *code_error);

    char *convert_float_to_str (float number);
#endif

#endif //MANDELBROT_HPP
