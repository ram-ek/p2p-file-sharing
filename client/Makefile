CC=g++
CFLAGS=-Wall -std=c++20

all: client

%.o: %.h %.cpp
	$(CC) $(CFLAGS) -c $^

client: cmd.o client.cpp
	$(CC) $(CFLAGS) -o client.out $^

clean:
	rm -rf *.o client