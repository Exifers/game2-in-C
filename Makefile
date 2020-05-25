all:
	gcc \
	main.c \
	io.c \
	globals.c \
  warn.c \
	classes/event.c \
	classes/vector.c \
	classes/color.c \
	classes/rect.c \
	classes/scene.c \
	classes/entity.c \
	load.c \
	-g -I/usr/include/SDL2 -I/usr/include -I. -lSDL2 -lm -o main
run:
	./main
