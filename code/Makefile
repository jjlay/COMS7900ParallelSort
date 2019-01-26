CC = mpicxx
CFLAGS = -std=c++11


parallelSort : parallelSort.o initializeMPI.o listFiles.o
	$(CC) $(CFLAGS) -o parallelSort parallelSort.o initializeMPI.o listFiles.o

parallelSort.o : parallelSort.cpp definitions.h
	$(CC) $(CFLAGS) -c parallelSort.cpp

initializeMPI.o : initializeMPI.cpp definitions.h
	$(CC) $(CFLAGS) -c initializeMPI.cpp

listFiles.o : listFiles.cpp
	$(CC) $(CFLAGS) -c listFiles.cpp


testbinsizing : testbinsizing.o
	$(CC) $(CFLAGS) -o testbinsizing testbinsizing.o

testbinsizing.o : testbinsizing.cpp
	$(CC) $(CFLAGS) -c testbinsizing.cpp

clean :
	rm -f testbinsizing parallelSort
	rm -f *.o


