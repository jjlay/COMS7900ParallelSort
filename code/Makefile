CC = mpicxx
CFLAGS = -std=c++11 -ggdb


<<<<<<< HEAD
parallelSort : parallelSort.o initializeMPI.o listFiles.o distributeFiles.o receiveFiles.o importFiles.o transmitMinMax.o getLinearBins.o adaptBins.o testUniformity.o binData.o
	$(CC) $(CFLAGS) -o parallelSort parallelSort.o initializeMPI.o listFiles.o distributeFiles.o receiveFiles.o importFiles.o transmitMinMax.o getLinearBins.o adaptBins.o testUniformity.o binData.o
=======
parallelSort : parallelSort.o initializeMPI.o listFiles.o distributeFiles.o receiveFiles.o importFiles.o transmitMinMax.o getLinearBins.o adaptBins.o testUniformity.o transmitMinMax.o receiveMinMax.o
	$(CC) $(CFLAGS) -o parallelSort parallelSort.o initializeMPI.o listFiles.o distributeFiles.o receiveFiles.o importFiles.o getLinearBins.o adaptBins.o testUniformity.o transmitMinMax.o receiveMinMax.o
>>>>>>> 9500f0c9d33b36479529417c6bb891556b7bf8fe

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< 

testbinsizing : testbinsizing.o
	$(CC) $(CFLAGS) -o testbinsizing testbinsizing.o

testbinsizing.o : testbinsizing.cpp
	$(CC) $(CFLAGS) -c testbinsizing.cpp

clean :
	rm -f testbinsizing parallelSort
	rm -f *.o


