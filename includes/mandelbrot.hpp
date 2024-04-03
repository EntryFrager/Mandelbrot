#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "../includes/mandelbrot_user.hpp"
#include "../includes/dsl.hpp"

#define FPS_ON

const int HEIGHT = 600;
const int WIDTH  = 800;

const double MAX_RADIUS = 10;

const int N_MAX = 256;

const int VECTOR_SIZE = 4;

#ifdef FPS_ON
    typedef struct {
        float     frame_time   = 0;
        float     render_time  = 0;

        uint64_t  frame_ticks  = 0;
        uint64_t  render_ticks = 0;

        sf::Clock clock;
    } WindowPerf;
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

    bool auto_switch_color = false;

    bool use_avx   = false;
    bool use_array = false;

    int n_color = 0;
} WindowConfig;

typedef struct {
    double x_offset = 0;
    double y_offset = 0;
    double zoom     = 1;
} WindowPosition;

struct Window{
    bool window_is_create = false;

    WindowConfig window_config = {};

    #ifdef FPS_ON
        WindowPerf window_perf = {};
    #endif

    WindowPosition window_position = {};
};

void set_color_pixel (WindowConfig *window_config, const long long int belong_mandelbrot, const int ix, const int iy, int *code_error);

#endif //MANDELBROT_HPP
