#include "mandelbrot.hpp"
#include "mandelbrot_avx.hpp"

void create_window (WindowSet *window_set, const char *font_file_name, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        WINDOW.create (sf::VideoMode (WIDTH, HEIGHT), HEADING);
        IMAGE.create (WIDTH, HEIGHT);
        TEXTURE.create (WIDTH, HEIGHT);
        SPRITE.setTexture (TEXTURE);

        #ifdef FPS_ON
                if (!FONT_FPS.loadFromFile (font_file_name))
                {
                        *code_error |= ERR_FOPEN;
                }
        #endif

        FLAG_OFFSET_WINDOW = true;
        X_OFFSET = 0;
        Y_OFFSET = 0;
        SCOPE = 1;
}

void draw_window (WindowSet *window_set, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        while (window_set->window.isOpen ())
        {
                COUNT_FPS (
                {
                        event_process (window_set, code_error);

                        if (FLAG_OFFSET_WINDOW)
                        {
                                if (USE_AVX)
                                {
                                        draw_mandelbrot_avx (window_set, code_error);
                                }
                                else
                                {
                                        draw_mandelbrot (window_set, code_error);
                                }

                                ERR_RET ();

                                TEXTURE.update (IMAGE);

                                WINDOW.clear ();
                                WINDOW.draw (SPRITE);
                        }
                });

                if (FLAG_OFFSET_WINDOW)
                {
                        WINDOW.display ();
                }

                FLAG_OFFSET_WINDOW = false;
        }
}

void event_process (WindowSet *window_set, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        sf::Event event;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

        while (WINDOW.pollEvent (event))
        {
                switch (event.type)
                {
                        case (EventKeyPressed):
                        {
                                FLAG_OFFSET_WINDOW = true;

                                if (KeyPressed (KeyEscape))
                                {
                                        WINDOW.close ();
                                }
                                else if (KeyPressed (KeyLeft))
                                {
                                        X_OFFSET -= COEF_WINDOW_MOVE_R_L;
                                }
                                else if (KeyPressed (KeyRight))
                                {
                                        X_OFFSET += COEF_WINDOW_MOVE_R_L;
                                }
                                else if (KeyPressed (KeyUp))
                                {
                                        Y_OFFSET -= COEF_WINDOW_MOVE_R_L;
                                }
                                else if (KeyPressed (KeyDown))
                                {
                                        Y_OFFSET += COEF_WINDOW_MOVE_R_L;
                                }
                                else if (KeyPressed (KeyEqual))
                                {
                                        SCOPE /= COEF_WINDOW_MOVE_U_D;
                                }
                                else if (KeyPressed (KeyHyphen))
                                {
                                        SCOPE *= COEF_WINDOW_MOVE_U_D;
                                }
                                else if (KeyPressed (KeyEnter))
                                {
                                        USE_AVX = !USE_AVX;
                                }

                                break;
                        }
                        case (EventWindowClosed):
                        {
                                WINDOW.close ();
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

void draw_mandelbrot (WindowSet *window_set, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        for (int iy = 0; iy < HEIGHT; iy++)
        {
                for (int ix = 0; ix < WIDTH; ix++)
                {
                        double mandel_coord_x = (((double) ix + X_OFFSET) / WIDTH  * COEF_STRETCH_X - COEF_CARRY_X) * SCOPE;
                        double mandel_coord_y = (((double) iy + Y_OFFSET) / HEIGHT * COEF_STRETCH_Y - COEF_CARRY_Y) * SCOPE;

                        volatile int belong_mandelbrot = check_point_mandelbrot (mandel_coord_x, mandel_coord_y);
                        ERR_RET ();

                        sf::Color color ((belong_mandelbrot * 6) % N_MAX, 0, (belong_mandelbrot * 10) % N_MAX);
                        IMAGE.setPixel (ix, iy, color);
                }
        }
}

#ifdef FPS_ON
void count_fps (WindowSet *window_set, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        float time_sec = TIME.asSeconds ();

        FPS = 1 / time_sec;
}

void print_fps (WindowSet *window_set, int *code_error)
{
        my_assert (window_set != NULL, ERR_PTR);

        TEXT_FPS.setFont (FONT_FPS);
        TEXT_FPS.setCharacterSize (CHARACTER_SIZE);

        TEXT_FPS.setFillColor (BLUE_COLOR);

        TEXT_FPS.setString (convert_float_to_str (FPS));

        WINDOW.draw(TEXT_FPS);
}

char *convert_float_to_str (float number)
{
        int number_len = snprintf (NULL, 0, "%.2f", number);

        char *result = (char *) calloc (number_len + 1, sizeof (char));
        snprintf (result, number_len + 1, "%.2f", number);

        return result;
}

#endif

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
