#include "../include/user.hpp"

const char *FONT_FILE_NAME = "./include/arialmt.ttf";
const int   FONT_SIZE      = 24;

int main ()
{
        int code_error = 0;

        create_window(FONT_FILE_NAME, FONT_SIZE, &code_error);

        draw_window(&code_error);
        PRINT_ERROR

        return 0;
}
