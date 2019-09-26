#CC        = clang++
CC        = g++
CFLAGS    = -std=c++03 -Wall -Wfatal-errors -g -I../../include -I../../include/etl -I../../include/etl/profiles -DPROFILE_CPP03_GENERIC
INCLUDES  = ../../include/etl/hsm.h

all: testhsm

testhsm: testhsm.cpp $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm testhsm
