CC = mpicxx
CFLAGS = -std=c++11 -ggdb -fbounds-check -Warray-bounds


parallelSort : parallelSort.o initializeMPI.o listFiles.o distributeFiles.o receiveFiles.o importFiles.o transmitMinMax.o getLinearBins.o adaptBins.o testUniformity.o transmitMinMax.o receiveMinMax.o binData.o transmitBinEdges.o receiveBinCounts.o transmitUniformity.o receiveBinIndices.o exportResults.o sortArray.o min.o max.o LL_sort.o swapArrayParts.o cleanUp.o testSort.o binData2.o
	$(CC) $(CFLAGS) -o parallelSort parallelSort.o initializeMPI.o listFiles.o distributeFiles.o receiveFiles.o importFiles.o getLinearBins.o adaptBins.o testUniformity.o transmitMinMax.o receiveMinMax.o binData.o transmitBinEdges.o receiveBinCounts.o transmitUniformity.o receiveBinIndices.o exportResults.o sortArray.o min.o max.o LL_sort.o swapArrayParts.o cleanUp.o testSort.o binData2.o

%.o : %.cpp definitions.h
	$(CC) $(CFLAGS) -c $< 

testbinsizing : testbinsizing.o
	$(CC) $(CFLAGS) -o testbinsizing testbinsizing.o

testbinsizing.o : testbinsizing.cpp
	$(CC) $(CFLAGS) -c testbinsizing.cpp

clean :
	rm -f testbinsizing parallelSort
	rm -f *.o
	rm -f core.*
	rm -f MetropolisSort.e*
	rm -f MetropolisSort.o*


nolog :
	rm -f core.*
	rm -f MetropolisSort.e*
	rm -f MetropolisSort.o*
