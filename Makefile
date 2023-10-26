CC = gcc
CFLAGS := -Werror -Wfatal-errors -g
LDFLAGS =

SRC = main.c sTalkFunctions.c socketTools.c
OBJ = list.o $(SRC:.c=.o)
EXECUTABLE = s-talk

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) -pthread $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(filter-out list.o, $(OBJ)) $(EXECUTABLE)
	rm -f assignment2.zip

zip:
	rm -f assignment2.zip
	zip assignment2.zip main.c socketTools.c sTalkFunctions.c list.h socketTools.h sTalkFunctions.h list.o README.md Makefile