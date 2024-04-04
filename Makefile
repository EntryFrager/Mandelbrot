CC = g++

CFLAGS = -c -O3 -mavx -mavx2 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline		\
	-Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default		    	\
	-Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy            \
	-Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing   \
	-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

LIBS = -lsfml-graphics -lsfml-window -lsfml-system

INCLUDES = include/user.hpp include/render_frame.hpp include/mandelbrot.hpp include/mandelbrot_avx.hpp include/mandelbrot_array.hpp include/keys.h include/error.h include/error_name.h include/dsl.hpp

SOURCES = main.cpp render_frame.cpp mandelbrot.cpp mandelbrot_avx.cpp mandelbrot_array.cpp error.cpp

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = mandelbrot.exe

VPATH = ./src/

all: $(SOURCES) $(INCLUDES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

.cpp.o: $(INCLUDES) $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf *.o mandelbrot
	rm -rf mandelbrot.exe
	rm -rf mandelbrot.exe.log
	rm -rf mandelbrot.exe.log.dmp
