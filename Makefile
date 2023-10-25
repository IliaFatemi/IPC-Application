CFLAGS := -Werror -Wfatal-errors -g

all: main

main: main.c
	gcc $(CFLAGS) -o s-talk main.c sTalkFunctions.c

clean:
	rm -f s-talk s-talk.o