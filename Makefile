CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
DEBUG= -g3
SDLCFLAGS=`sdl2-config --cflags`
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm
SDLLIBS=`sdl2-config --libs`

parse: code/main.c code/general.h code/general.c code/turtle.h code/turtle.c code/stack.h code/stack.c
	$(CC)  code/main.c code/general.c  code/turtle.c code/stack.c -o parse -I./code $(SANITIZE)  $(LDLIBS) -DPARSE

interp: code/main.c code/general.h code/general.c code/turtle.h code/turtle.c code/stack.h code/stack.c code/neillsdl2.c code/neillsdl2.h
	$(CC)  code/main.c code/general.c code/neillsdl2.c code/turtle.c code/stack.c -o interp -I./code $(PRODUCTION) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS) -DINTERP

test1: test/test.c code/general.h code/general.c code/turtle.h code/turtle.c code/stack.h code/stack.c
	$(CC)  test/test.c code/general.c  code/turtle.c code/stack.c -o test1 -I./code $(SANITIZE)  $(LDLIBS) -DPARSE

test2: test/test.c code/general.h code/general.c code/turtle.h code/turtle.c code/stack.h code/stack.c code/neillsdl2.c code/neillsdl2.h
	$(CC)  test/test.c code/general.c  code/neillsdl2.c code/turtle.c code/stack.c -o test2 -I./code $(PRODUCTION) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS) -DINTERP

clean:
	rm -f parse interp test1 test2
