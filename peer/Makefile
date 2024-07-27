CC=g++
CFLAGS=-Wall -std=c++20

all: peer

%.o: %.h %.cpp
	$(CC) $(CFLAGS) -c $^

peer: cmd.o utils.o peer.cpp
	$(CC) $(CFLAGS) -o peer.out $^

clean:
	rm -rf *.o peer.out