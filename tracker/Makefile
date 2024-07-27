CC=g++
CFLAGS=-Wall -std=c++20

all: tracker

%.o: %.h %.cpp
	$(CC) $(CFLAGS) -c $^

tracker: cmd.o server.o utils.o tracker.cpp
	$(CC) $(CFLAGS) -o tracker.out $^

clean:
	rm -rf *.o tracker.out