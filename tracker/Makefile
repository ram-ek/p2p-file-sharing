CC=g++
CFLAGS=-Wall -std=c++20
OBJS = \
	console.o \
	commands.o \
	cmdcall.o \
	server.o \
	utils.o

all: main

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

main: $(OBJS) main.cpp
	$(CC) $(CFLAGS) -o main.out $^

clean:
	rm -rf *.o main.out