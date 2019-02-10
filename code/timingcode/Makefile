file1test : file1test.o
	mpicc -std=c++11 -o file1test file1test.o ../initializeMPI.o ../receiveFiles.o ../listFiles.o ../importFiles.o -lstdc++ ../distributeFiles.o


file2test : file2test.o
	mpicc -std=c++11 -o file2test file2test.o ../initializeMPI.o ../receiveFiles.o ../listFiles.o ../importFiles.o -lstdc++ ../distributeFiles.o


file1test.o : file1test.cpp
	mpicc -std=c++11 -c file1test.cpp


file2test.o : file2test.cpp
	mpicc -std=c++11 -c file2test.cpp -I $(HOME)/COMS7900ParallelSort/code/

