CFLAGS := -Werror -Wfatal-errors -g

all: main

main: main.c
	gcc $(CFLAGS) -o $@ list.o main.c s-talk.c

clean:
	rm -f main main.o