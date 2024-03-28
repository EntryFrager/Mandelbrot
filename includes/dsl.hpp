#ifndef DSL_H
#define DSL_H

#define BLUE_COLOR         sf::Color::Blue

#define WINDOW             window_set->window
#define IMAGE              window_set->image
#define TEXTURE            window_set->texture
#define SPRITE             window_set->sprite

#define USE_AVX            window_set->use_avx

#define FONT_FPS           window_set->font_fps
#define TEXT_FPS           window_set->text_fps
#define FPS                window_set->fps
#define CLOCK              window_set->clock_info.clock
#define TIME               window_set->clock_info.time

#define FLAG_OFFSET_WINDOW window_set->flag_offset_window
#define X_OFFSET           window_set->x_offset
#define Y_OFFSET           window_set->y_offset
#define SCOPE              window_set->scope

#define EventKeyPressed   sf::Event::KeyPressed
#define EventWindowClosed sf::Event::Closed
#define KeyPressed(Key)   sf::Keyboard::isKeyPressed (Key)
#define KeyLeft           sf::Keyboard::Left
#define KeyRight          sf::Keyboard::Right
#define KeyUp             sf::Keyboard::Up
#define KeyDown           sf::Keyboard::Down
#define KeyEqual          sf::Keyboard::Equal
#define KeyHyphen         sf::Keyboard::Hyphen
#define KeyEnter          sf::Keyboard::Enter
#define KeyEscape         sf::Keyboard::Escape

// #define KeyNum1           sf::Keyboard::Num1
// #define KeyNum2           sf::Keyboard::Num2
// #define KeyNum3           sf::Keyboard::Num3
// #define KeyNum4           sf::Keyboard::Num4
// #define KeyNum5           sf::Keyboard::Num5
// #define KeyNum6           sf::Keyboard::Num6
// #define KeyNum7           sf::Keyboard::Num7
// #define KeyNum8           sf::Keyboard::Num8
// #define KeyNum9           sf::Keyboard::Num9


#endif //DSL_H
