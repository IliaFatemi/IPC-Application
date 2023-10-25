

CC = gcc
CFLAGS := -Werror -Wfatal-errors -g
LDFLAGS =

SRC = main.c sTalkFunctions.c
OBJ = list.o $(SRC:.c=.o)
EXECUTABLE = s-talk

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(filter-out list.o, $(OBJ)) $(EXECUTABLE)