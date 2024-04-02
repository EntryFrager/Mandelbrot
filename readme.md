# Проект на тему: "***Оптимизации кода с использованием векторных инструкций, на примере рендеринга множества Мандельброта***"

## Введение

Данный проект содержит программу по отрисовке множества Мандельброта с разными способами рендеринга. Целью проекта является исследовать эффективность кода с использованием `SIMD` отпимизаций и без них. Также проверить, что будет, если использовать флаг `-O3`. Сравнить полученные результаты.

## Скачивание и установка программы

Для установки программы используются утилиты `make` и `git`. Пропиши в командной строке:

```CMake
    git clone git@github.com:EntryFrager/Mandelbrot.git
    cd Mandelbrot
    make
    ./mandelbrot
```

> [!IMPORTANT]
>
> Данная программа предназначена для работы на процессорах поддерживающих `AVX\AVX2` инструкции (Advanced Vector Extensions).

## Немного про множество Мандельброта

Мно́жество Мандельбро́та — множество точек c на комплексной плоскости, для которых рекуррентное соотношение задаёт ограниченную последовательность.

$$ x_ n = x_ {n-1} ^ 2 - y_ {n-1} ^ 2 + x_ 0 $$
$$ y_ n = 2 * x_ {n-1} * y_ {n-1} + y_ 0 $$

> Данная информация взята с [этого сайта](https://ru.wikipedia.org/wiki/%D0%9C%D0%BD%D0%BE%D0%B6%D0%B5%D1%81%D1%82%D0%B2%D0%BE_%D0%9C%D0%B0%D0%BD%D0%B4%D0%B5%D0%BB%D1%8C%D0%B1%D1%80%D0%BE%D1%82%D0%B0)

## О программе

Для рисования множества Мандельброта была использована библиотека `SFML`.

![overflow](/picture_for_readme/1.png)

### Интерфейс программы

Интерфейс программы содержит информацию о:
1. Значении времени отрисовки одного кадра в FPS и тиках процессора.
2. Значении времени рендера одного кадра в FPS и тиках процессора.

### Горячие клавиши

| Клавиша           |     Действие                         |
|-------------------|--------------------------------------|
|   Q               |Наивный метод рендера                 |
|   W               |Метод рендера на основе массивов      |
|   E               |Метод рендера на основе avx инструкций|
| :arrow_up:        |Сдвиг вверх                           |
| :arrow_down:      |Сдвиг вниз                            |
| :arrow_left:      |Сдвиг влево                           |
| :arrow_right:     |Сдвиг вправо                          |
|   +               |Увеличение изображения                |
|   -               |Уменьшение изображения                |
| 1, ... , 9        |Переключение цветов                   |
|Mouse wheel up     |Следующий цвет                        |
|Mouse wheel down   |Предыдущий цвет                       |

## Оптимизации

Для оптимизации данной программы были применены `AVX/AVX2` инструкции, метод отпимизации, путем создания массива из 4 чисел с плавающей точкой двойной точности, а также при помощи использования флага `-O3` при компилировании программы.

### AVX/AVX2

Данная архитектура поддерживает шестнадцать 256-битных регистров `YMM`. Поддерживаются целочисленные типы данных и типы с плавающей точкой.

Архитектура AVX поддерживают два основных типа данных: скалярные значения (отдельные числа) и векторы (наборы чисел). Скалярные значения представляют отдельные значения с плавающей точкой одинарной или двойной точности. Векторы содержат несколько значений целых или вещественных чисел.

Для получения более точной картины множества мандельброта в данной работе использовались векторы, содержащие 4 числа с плавающей точкой двойной точности.

> [!IMPORTANT]
>
> Метод оптимизации на массивах содержит 4 числа, так как размер вектор равен 256 битам.

## Измерения

Для измерения времени отрисовки был использован класс для измерения времени `sf::Clock` из библиотеки `SFML`. При помощи функции `clock.restart()` производился перезапуск таймера. А для получения времения использовалась функция `clock.getElapsedTime()`. Функцией `asSeconds()` наше время, полученное предыдущей функцией, преобразовывалось в секунды.

Для измерения в тиках процессора была использована функция `__rdtsc()`. TSC (time stamp counter) — это аппаратный счетчик, используемый во всех современных устройствах, который увеличивается при каждом такте процессора. Чтобы измерить количество тиков, затраченных на выполнение участка кода, мы должны знать количество тиков в начале выполнения кода, и в конце. Тогда на выполнение нашего кода будет затрачена разность данных величин.

Ниже в таблицах приведены средние значения исследуемых величин.

### Время отрисовки кадра в FPS

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 5.57 $                 | $ 1.32 $                     | $ 9.66 $         |
|           -O3       | $ 10.36 $                | $ 7.18 $                     | $ 33.34 $        |

### Время отрисовки кадра в тиках процессора

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 465294546 $            | $ 1967992453 $               | $ 268454473 $    |
|           -O3       | $ 257249258 $            | $ 361179325 $                | $ 77748987 $     |

### Время рендеринга кадра в FPS

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 5.59 $                 | $ 1.32 $                     | $ 9.71  $        |
|           -O3       | $ 10.43 $                | $ 7.21 $                     | $ 34.08 $        |

### Время рендеринга кадра в тиках процессора

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 463640378 $            | $ 1966291049 $               | $ 266836936 $    |
|           -O3       | $ 248926321 $            | $ 359567004  $               | $ 76068153  $    |

## Расчет погрешностей

Для расчета погрешностей воспользуемся данной формулой:

$$ \sigma_ {x_ {cр}} = \sqrt \frac {\Sigma_ {i=1} ^ n(x_ {i} - x_ {ср}) ^ 2}{n * (n - 1)}$$

### Погрешность времени отрисовки кадра в FPS

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 0.038 $                | $ 0.005 $                    | $ 0.006 $        |
|           -O3       | $ 0.170 $                | $ 0.081 $                    | $ 0.128 $        |

### Погрешность времени отрисовки кадра в тиках процессора

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 3.18 * 10 ^ 6 $        | $ 7.22 * 10 ^ 6 $            | $ 0.52 * 10 ^ 6 $|
|           -O3       | $ 1.79 * 10 ^ 6 $        | $ 4.20 * 10 ^ 6 $            | $ 0.31 * 10 ^ 6 $|

### Погрешность времени рендеринга кадра в FPS

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 0.039 $                | $ 0.005 $                    | $ 0.009 $        |
|           -O3       | $ 0.071 $                | $ 0.084 $                    | $ 0.133 $        |

### Погрешность времени рендеринга кадра в тиках процессора

| Уровень оптимизации | Наивный метод рендеринга | Метод основанный на массивах | SIMD инструкции  |
|---------------------|--------------------------|------------------------------|------------------|
|           -         | $ 3.23 * 10 ^ 6 $        | $ 7.17 * 10 ^ 6 $            | $ 0.24 * 10 ^ 6 $|
|           -O3       | $ 1.77 * 10 ^ 6 $        | $ 3.98 * 10 ^ 6 $            | $ 0.31 * 10 ^ 6 $|

## Вывод
Таким образом, в ходе выполнения данной работы мы рассмотрели три метода рендеринга кадра:
1. Наивный метод;
2. Метод основанный на массивах;
3. `SIMD` инструкции.

Среди этих способов, самым эффективным оказался метод основанный на `SIMD` инструкциях (single instruction multiple data). При использовании `SIMD` инструкций эффективность кода заметно повышается в 2 - 3 раза. Если при компиляции программы добавить флаг `-O3`, то эффективность нашего кода повысится в 2 - 3 раза. А если применить оба вышеописанных метода, то эффективность нашего кода повысится в 2 - 6 раз.

Метод основанный на массивах показал себя, как самый низкоэффективный способ оптимизации кода. Данный метод не оптимизировал код, а наоборот, понизил его эффективность. При компиляции данного метода с флагом `-O3` прирост производительности был в ~7 раз.
