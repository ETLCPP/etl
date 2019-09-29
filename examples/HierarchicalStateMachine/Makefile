#CC        = clang++
CC        = g++

# -permissive required to allow for specialisation of hsm in different namespace
CFLAGS    = -std=c++03 -fpermissive -Wall -Wfatal-errors -g -I./ -I../../include
INCLUDES  = ../../include/etl/hsm.h

all: testhsm

testhsm: testhsm.cpp $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm testhsm
