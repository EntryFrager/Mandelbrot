#include "mandelbrot.hpp"
#include "mandelbrot_avx.hpp"

int main ()
{
        int code_error = 0;
        const char *font_file_name = "./includes/arialmt.ttf";

        WindowSet window_set = {};
        create_window (&window_set, font_file_name, &code_error);

        draw_window (&window_set, &code_error);
        PRINT_ERROR

        return 0;
}
