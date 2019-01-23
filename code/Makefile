parallelSort : parallelSort.o
	mpicc -o parallelSort parallelSort.o

parallelSort.o : parallelSort.cpp definitions.h
	mpicc -c parallelSort.cpp


testbinsizing : testbinsizing.o
	g++ - o testbinsizing testbinsizing.o

testbinsizing.o : testbinsizing.cpp
	g++ - c testbinsizing.cpp

clean :
	rm - f testbinsizing parallelSort
	rm - f * .o
