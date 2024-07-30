CC=g++
CFLAGS=-Wall -std=c++20

all: main

%.o: %.h %.cpp
	$(CC) $(CFLAGS) -c $^

main: console.o server.o utils.o main.cpp
	$(CC) $(CFLAGS) -o main.out $^

clean:
	rm -rf *.o main.out