
CC := gcc
CFLAGS := -g -pg -Wall -Werror
OBJECTS := mindi.o

all: mindi

mindi: $(OBJECTS) main.o
	$(CC) -o $@ $^ $(CFLAGS)

.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o mindi

