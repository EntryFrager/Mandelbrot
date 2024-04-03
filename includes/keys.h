DEF_KEY(KeyEscape,
        {
            window->window_config.window.close();
        })

DEF_KEY(KeyEnter,
        {
            window->window_config.auto_switch_color = !window->window_config.auto_switch_color;
        })

DEF_KEY(KeyLeft,
        {
            window->window_position.x_offset += COEF_WINDOW_MOVE / window->window_position.zoom;
        })

DEF_KEY(KeyRight,
        {
            window->window_position.x_offset -= COEF_WINDOW_MOVE / window->window_position.zoom;
        })

DEF_KEY(KeyUp,
        {
            window->window_position.y_offset += COEF_WINDOW_MOVE / window->window_position.zoom;
        })

DEF_KEY(KeyDown,
        {
            window->window_position.y_offset -= COEF_WINDOW_MOVE / window->window_position.zoom;
        })

DEF_KEY(KeyEqual,
        {
            window->window_position.zoom *= COEF_WINDOW_ZOOM;
        })

DEF_KEY(KeyHyphen,
        {
            window->window_position.zoom /= COEF_WINDOW_ZOOM;
        })

DEF_KEY(KeyQ,
        {
            window->window_config.use_avx = false;

            window->window_config.use_array = false;
        })

DEF_KEY(KeyW,
        {
            window->window_config.use_avx = false;

            window->window_config.use_array = true;
        })

DEF_KEY(KeyE,
        {
            window->window_config.use_avx = true;

            window->window_config.use_array = false;
        })

DEF_KEY(KeyNum1,
        {
            window->window_config.n_color = 0;
        })

DEF_KEY(KeyNum2,
        {
            window->window_config.n_color = 1;
        })

DEF_KEY(KeyNum3,
        {
            window->window_config.n_color = 2;
        })

DEF_KEY(KeyNum4,
        {
            window->window_config.n_color = 3;
        })

DEF_KEY(KeyNum5,
        {
            window->window_config.n_color = 4;
        })

DEF_KEY(KeyNum6,
        {
            window->window_config.n_color = 5;
        })

DEF_KEY(KeyNum7,
        {
            window->window_config.n_color = 6;
        })

DEF_KEY(KeyNum8,
        {
            window->window_config.n_color = 7;
        })

DEF_KEY(KeyNum9,
        {
            window->window_config.n_color = 8;
        })
