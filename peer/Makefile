CC=g++
CFLAGS=-Wall -std=c++20
OBJS = \
	console.o \
	create_user.o \
	login.o \
	create_group.o \
	join_group.o \
	leave_group.o \
	list_requests.o \
	logout.o \
	quit.o \
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