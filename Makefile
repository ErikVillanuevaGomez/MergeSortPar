CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++17 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
LD=g++

all: mergesort

mergesort: main.o
		$(LD) $(LDFLAGS) main.o -o mergesort

main.o: main.cpp
		$(LD) $(CXXFLAGS) -c main.cpp

clean:
		-rm *.o mergesort
