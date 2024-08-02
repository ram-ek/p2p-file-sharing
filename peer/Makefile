CC=g++
CFLAGS=-Wall -std=c++20
OBJS = console.o create_user.o login.o create_group.o join_group.o logout.o commands.o cmdcall.o server.o utils.o main.cpp

all: main

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

main: $(OBJS)
	$(CC) $(CFLAGS) -o main.out $^

clean:
	rm -rf *.o main.out