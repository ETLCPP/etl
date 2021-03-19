CC        = clang++
#CC        = g++
CFLAGS    = -std=c++17 \
			-Wall -Wfatal-errors \
			-g \
			-I../../include

all: testhsm

testhsm: testhsm.cpp $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm testhsm
