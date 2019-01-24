CC = mpicxx
CFLAGS = -std=c++11


parallelSort : parallelSort.o
	$(CC) $(CFLAGS) -o parallelSort parallelSort.o

parallelSort.o : parallelSort.cpp definitions.h
	$(CC) $(CFLAGS) -c parallelSort.cpp


testbinsizing : testbinsizing.o
	$(CC) $(CFLAGS) -o testbinsizing testbinsizing.o

testbinsizing.o : testbinsizing.cpp
	$(CC) $(CFLAGS) -c testbinsizing.cpp

clean :
	rm -f testbinsizing parallelSort
	rm -f *.o


