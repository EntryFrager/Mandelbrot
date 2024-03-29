DEF_KEY(KeyEscape,
        {
            window->window_config.window.close();
        })

DEF_KEY(KeyLeft,
        {
            window->window_position.x_offset -= COEF_WINDOW_MOVE;
        })

DEF_KEY(KeyRight,
        {
            window->window_position.x_offset += COEF_WINDOW_MOVE;
        })

DEF_KEY(KeyUp,
        {
            window->window_position.y_offset -= COEF_WINDOW_MOVE;
        })

DEF_KEY(KeyDown,
        {
            window->window_position.y_offset += COEF_WINDOW_MOVE;
        })

DEF_KEY(KeyEqual,
        {
            window->window_position.zoom /= COEF_WINDOW_ZOOM;
        })

DEF_KEY(KeyHyphen,
        {
            window->window_position.zoom *= COEF_WINDOW_ZOOM;
        })

DEF_KEY(KeyEnter,
        {
            window->window_config.use_avx = !window->window_config.use_avx;
        })

DEF_KEY(KeyNum1,
        {
            window->window_config.color[0] = COLORS[0][0];
            window->window_config.color[1] = COLORS[0][1];
        })

DEF_KEY(KeyNum2,
        {
            window->window_config.color[0] = COLORS[1][0];
            window->window_config.color[1] = COLORS[1][1];
        })

DEF_KEY(KeyNum3,
        {
            window->window_config.color[0] = COLORS[2][0];
            window->window_config.color[1] = COLORS[2][1];
        })

DEF_KEY(KeyNum4,
        {
            window->window_config.color[0] = COLORS[3][0];
            window->window_config.color[1] = COLORS[3][1];
        })

DEF_KEY(KeyNum5,
        {
            window->window_config.color[0] = COLORS[4][0];
            window->window_config.color[1] = COLORS[4][1];
        })

DEF_KEY(KeyNum6,
        {
            window->window_config.color[0] = COLORS[5][0];
            window->window_config.color[1] = COLORS[5][1];
        })

DEF_KEY(KeyNum7,
        {
            window->window_config.color[0] = COLORS[6][0];
            window->window_config.color[1] = COLORS[6][1];
        })

DEF_KEY(KeyNum8,
        {
            window->window_config.color[0] = COLORS[7][0];
            window->window_config.color[1] = COLORS[7][1];
        })

DEF_KEY(KeyNum9,
        {
            window->window_config.color[0] = COLORS[8][0];
            window->window_config.color[1] = COLORS[8][1];
        })
