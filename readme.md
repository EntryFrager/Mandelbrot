# ***Оптимизации кода с использованием векторных инструкций, на примере расчета множества Мандельброта***

## Введение

Данный проект содержит программу рендеринга множества Мандельброта с разными способами его расчета. Мы исследуем влияние `SIMD` отпимизаций и флага `-O3` на быстродействие кода.

## Скачивание и установка программы

Для установки программы используются утилиты `make` и `git`.

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

$$ y_ n = 2 \cdot x_ {n-1} \cdot y_ {n-1} + y_ 0 $$

> Данная информация взята с [этого сайта](https://ru.wikipedia.org/wiki/%D0%9C%D0%BD%D0%BE%D0%B6%D0%B5%D1%81%D1%82%D0%B2%D0%BE_%D0%9C%D0%B0%D0%BD%D0%B4%D0%B5%D0%BB%D1%8C%D0%B1%D1%80%D0%BE%D1%82%D0%B0).

## О программе

Для рендеринга множества Мандельброта была использована библиотека `SFML`.

![overflow](/picture_for_readme/mandelbrot.gif)

### Интерфейс программы

Интерфейс программы содержит информацию о:
1. Значении времени рендеринга одного кадра в FPS и тиках процессора
2. Значении времени расчета множества Мандельброта в FPS и тиках процессора

### Горячие клавиши

| Клавиша           |     Действие                         |
|-------------------|--------------------------------------|
|   Q               |Наивный метод расчета                 |
|   W               |Метод расчета на основе массивов      |
|   E               |Метод расчета на основе avx инструкций|
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

Для оптимизации данной программы были применены
1. `AVX/AVX2` инструкции
2. Метод отпимизации, основанный на массивах из 4 чисел типа `double`
3. Использования флага `-O3` при компиляции программы

### AVX/AVX2

Данная архитектура поддерживает шестнадцать 256-битных регистров `YMM`. Поддерживаются целочисленные типы данных и типы с плавающей точкой.

Основным типом данных архитектура AVX являются векторы (наборы чисел). Векторы содержат несколько значений целых или вещественных чисел. Регистры YMM (YMM0-YMM15) могут хранить 8 чисел типа `float` (32-разрядных) или 4 числа типа `double` (64-разрядных).

Для получения более точной картины множества мандельброта в данной работе использовались векторы, содержащие 4 числа типа `double`.

> [!IMPORTANT]
>
> Метод оптимизации на массивах содержит 4 числа, так как размер вектора равен 256 битам.

> Более подробно про `SIMD` инструкции вы можете прочитать [здесь](https://ru.wikipedia.org/wiki/SIMD).

## Измерения

Для измерений в FPS (количество кадров в секунду) был использован класс для измерения времени `sf::Clock` из библиотеки `SFML`. Документация находится [здесь](https://www.sfml-dev.org/tutorials/2.6/system-time.php).

Для измерений в тиках процессора была использована функция `__rdtsc()`. Более подробно про данную функцию вы можете прочитать [здесь](https://man.netbsd.org/NetBSD-8.1/x86/rdtsc.9).

Ниже в таблицах приведены средние значения для `n = 50` серий измерений.

### Время рендеринга кадра и расчета множества Мандельброта в FPS

|Метод расчета множества Мандельброта | Уровень оптимизации | Рендеринга кадра | Расчет множества Мандельброта  |
|-------------------------------------|---------------------|------------------|--------------------------------|
| Наивный метод                       |         -O2         | 5.40             | 5.42                           |
| Наивный метод                       |         -O3         | 10.25            | 10.32                          |
| Метод основанный на массивах        |         -O2         | 1.50             | 1.50                           |
| Метод основанный на массивах        |         -O3         | 7.82             | 7.86                           |
| SIMD инструкции                     |         -O2         | 9.57             | 9.64                           |
| SIMD инструкции                     |         -O3         | 33.20            | 33.93                          |

### Время рендеринга кадра и расчета множества Мандельброта в тиках процессора $\cdot 10^6$

|Метод расчета множества Мандельброта | Уровень оптимизации | Рендеринга кадра | Расчет множества Мандельброта  |
|-------------------------------------|---------------------|------------------|--------------------------------|
| Наивный метод                       |         -O2         | 480.80           | 478.93                         |
| Наивный метод                       |         -O3         | 252.88           | 251.13                         |
| Метод основанный на массивах        |         -O2         | 1725.3           | 1723.47                        |
| Метод основанный на массивах        |         -O3         | 331.52           | 329.89                         |
| SIMD инструкции                     |         -O2         | 270.94           | 269.19                         |
| SIMD инструкции                     |         -O3         | 78.08            | 76.39                          |

## Расчет погрешностей

Для расчета абсолютной погрешности и относительной воспользуемся данными формулами:

$$ \sigma_ {x_ {cр}} = \sqrt \frac {\Sigma_ {i=1} ^ n(x_ {i} - x_ {ср}) ^ 2}{n \cdot (n - 1)}$$

$$ \varepsilon = \frac {\sigma_ {x_ {cр}}} {x_ {ср}} \cdot 100\% $$

Полная таблица с погрешностями находится в файле [data.md](https://github.com/EntryFrager/Mandelbrot/blob/Main/data.md).

## Обсуждение результатов

Исходя из полученных результатов, мы можем заметить, что время рендеринга кадра не сильно отличается от времени расчета множества Мандельброта. Таким образом, главную роль в быстродействии кода играет расчет множества Мандельброта.

Переводя тики процессора в FPS по данной формуле:

$$ FPS = \frac {2.6 \cdot 10 ^ 9} {ticks} $$

Мы понимаем, что измерения в тиках процессора и в FPS одинаковы, из чего следует, что наши измерения верные.

Ниже приведена таблица с величинами, которые характеризуют увеличение быстродействие кода. Наивный метод рендеринга, с уровнем оптимизации -O2 будем считать за `1`, а все остальные величины будем расчитывать относительно нее.

|Метод расчета множества Мандельброта | Уровень оптимизации | Рендеринга кадра |
|-------------------------------------|---------------------|------------------|
| Наивный метод                       |         -O2         | 1                |
| Наивный метод                       |         -O3         | 1.9              |
| Метод основанный на массивах        |         -O2         | 0.3              |
| Метод основанный на массивах        |         -O3         | 1.5              |
| SIMD инструкции                     |         -O2         | 1.8              |
| SIMD инструкции                     |         -O3         | 6.1              |

## Вывод

Таким образом, в ходе выполнения данной работы мы рассмотрели три метода расчета множества Мандельброта:

1. Наивный метод
2. Метод основанный на массивах
3. `SIMD` инструкции

Если при компиляции программы добавить флаг `-O3`, то быстродействие нашего кода повысится в ~2 раза для наивного метода.

При использовании `SIMD` инструкций скорость выполнения нашей программы заметно повышается в ~2 раза.  А если применить `SIMD` инструкции и добавить флаг `-O3`, то быстродействие нашего кода повысится в ~6 раз.

Метод основанный на массивах показал себя, как самый низкоэффективный способ оптимизации кода. Данный метод не оптимизировал код, а наоборот, понизил его быстродействие, сделав ещё медленнее в ~0.3 раза. При компиляции данного метода с флагом `-O3` прирост производительности был в ~5 раз относительно, компиляции данного метода с уровнем оптимизации `-O3`.

Таким образом среди всех рассмотренных методов расчета кадра, самым эффективным оказался метод, использующий `AVX\AVX2` инструкции.

## Источники информации

1. https://ru.wikipedia.org/wiki/%D0%9C%D0%BD%D0%BE%D0%B6%D0%B5%D1%81%D1%82%D0%B2%D0%BE_%D0%9C%D0%B0%D0%BD%D0%B4%D0%B5%D0%BB%D1%8C%D0%B1%D1%80%D0%BE%D1%82%D0%B0
2. https://www.laruence.com/sse/#
3. https://www.sfml-dev.org/tutorials/2.6/

