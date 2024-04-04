#include "../include/user.hpp"

const char *FONT_FILE_NAME = "./include/arialmt.ttf";

int main ()
{
        int code_error = 0;
        const char *font_file_name = FONT_FILE_NAME;

        create_window(font_file_name, &code_error);

        draw_window(&code_error);
        PRINT_ERROR

        return 0;
}
