# -*- Makefile -*-
all: gauss
gauss: gauss.cpp
	g++ gauss.cpp -o gauss
clean:
	rm -f *.x *.o