#include "..//includes/mandelbrot.hpp"

int main ()
{
        int code_error = 0;
        const char *font_file_name = FONT_FILE_NAME;

        Window window = {};
        create_window(&window, font_file_name, &code_error);

        draw_window(&window, &code_error);
        PRINT_ERROR

        return 0;
}
