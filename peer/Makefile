CC=g++
CFLAGS=-Wall -std=c++20

all: main

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

main: console.o create_user.o login.o logout.o commands.o cmdcall.o server.o utils.o main.cpp
	$(CC) $(CFLAGS) -o main.out $^

clean:
	rm -rf *.o main.out