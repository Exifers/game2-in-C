CC := gcc
CFLAGS :=
CPPFLAGS := -I/usr/include/SDL2 -I/usr/include -I.
LDFLAGS := -lm -lSDL2

debug : CFLAGS += -g

src := $(shell find . -type f -name '*.c')
obj := $(src:%.c=%.o)
bin := main

all: $(obj)
	$(CC) $^ $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $(bin)

run: all
	./$(bin)

debug: $(obj)
	$(CC) $^ $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $(bin)

clean:
	$(RM) $(obj) $(bin)

.PHONY: all run clean
