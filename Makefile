CFLAGS := -Werror -Wfatal-errors -g

all: main

main: list.c
	gcc $(CFLAGS) -o $@ list.c main.c s-talk.c

clean:
	rm -f main main.o