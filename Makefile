CC=gcc
CFLAGS=-Wall -Wextra -g -target arm64-apple-macos11

all: main 

main: main.c 
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f main

.PHONY: all clean
