//
// Parallel Sort
//

//
// Standard includes
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono> 


//
// Parallel includes
//

#include "mpi.h"


//
// Local includes
//

#include "definitions.h"
#include "initializeMPI.h"
#include "listFiles.h"
#include "distributeFiles.h"
#include "receiveFiles.h"
#include "importFiles.h"
#include "getLinearBins.h"
// #include "binData.h"   // not written yet
#include "adaptBins.h"
#include "testUniformity.h"
#include "receiveMinMax.h"
#include "transmitMinMax.h"


// #include "Data.h"


//
// Function: main
//

int main(int argc, char *argv[])
{
	//
	// MPI setup
	//

	std::string processorName;
	int myRank, numNodes;

	initializeMPI(&processorName, &myRank, &numNodes, argc, argv);

#ifdef _TIMING_
	auto timeStart = std::chrono::system_clock::now();
#endif
	
	int numWorkers = numNodes - 1;
	
	// set cout to print doubles' full length
	std::cout.precision(17);
	
	
	std::cout << "Rank " << myRank
		<< " running on " << processorName 
		<< " with " << numNodes << " total processes" 
		<< std::endl;

#ifdef _TIMING_
	auto timeBeginFilenameDistribute = std::chrono::system_clock::now();
	std::chrono::duration<double> timeElapsedSeconds = timeBeginFilenameDistribute - timeStart;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to initialize MPI" << std::endl;
#endif

	// Change the following variable to the actual
	// location of the data files
	std::string path = "./data/";
	
	std::vector<std::string> FilenameArray;

	if (myRank == 0)
		FilenameArray = listFiles(path);

	if (myRank == 0) {
		// Distribute files
		std::cout << "Rank 0 before distribute files" << std::endl;
		distributeFiles(FilenameArray, numNodes-1);

		std::cout << "Rank 0 after distribute files" << std::endl;
	} 
	else {
		// Receive file list
		FilenameArray = receiveFiles(myRank);
		std::cout << "Rank " << myRank << " after distribute files" << std::endl;
	}

#ifdef _TIMING_
	auto timeBeginFileImport = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginFileImport - timeBeginFilenameDistribute;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to distribute the filenames" << std::endl;

#endif

	double myMin = 0.0;
	double myMax = 0.0;
	double *array;

	if (myRank != 0) {
		// Read data files in
		// Data_COMS *myData;
		// myData = importFiles( FilenameArray, myRank );
		//std::cout << myData << std::endl;

		unsigned int arraySize = FilenameArray.size() * maxRows * _ROW_WIDTH_;
		std::cout << "Rank " << myRank << " will allocate " << arraySize << " elements" << std::endl;

		array = new double[FilenameArray.size() * maxRows * _ROW_WIDTH_]; //JJL
		int rows = 0, cols = 0;
		
//		std::cout << "Rank " << myRank << " is importing files" << std::endl;
		importFiles(FilenameArray, myRank, array, &rows, &cols);
//		std::cout << "rank: " << myRank << " " << array[0] << " " << array[1] << " " << array[2] << " " << array[3] << std::endl;
//		std::cout << "Rank " << myRank << " has imported files" << std::endl;
		
		// Perform initial sort
	}
	
	
	std::cout << "Rank " << myRank << " has reached the barrier" << std::endl;

	MPI_Barrier(MPI_COMM_WORLD);

	std::cout << "Rank " << myRank << " has passed the barrier" << std::endl;

#ifdef _TIMING_	
	auto timeBeginMinMax = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginMinMax - timeBeginFileImport;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to import data" << std::endl;
#endif

	std::cout << "Rank " << myRank << " says numNodes = " << numNodes << std::endl;

	auto allMins = new double[numNodes];
	auto allMaxs = new double[numNodes];

	if (myRank == 0) {
		// Receive minimums and maximums
		allMins[Rank0] = 0.0;
		allMaxs[Rank0] = 0.0;

		for (auto r = 1; r < numNodes; r++) {
			receiveMinMax(r, &allMins[r], &allMaxs[r]);
			std::cout << "Rank " << r << " send " 
				<< allMins[r] << " and "
				<< allMaxs[r] << std::endl;
		}
	} 
	else {
		// Send minimums and maximums
		transmitMinMax(myMin, myMax);
	}
	
	

#ifdef _TIMING_	
	auto timeBeginBinning = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginBinning - timeBeginMinMax;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to exchange min and max" << std::endl;
#endif

	if (myRank == 0) {
		double *binE = new double[numWorkers+1]; // GW
		double minGlobal = -1.0, maxGlobal = 1.0; // remove -1,+1 when done testing
		
		// Calculate bins
		getLinearBins( binE, numNodes-1, myRank, minGlobal, maxGlobal );
		std::cout << "binE 0: " << binE[0] << " " << binE[1] << " " << binE[2] << " " << binE[3] << std::endl;
		
		int binC[3] = { 140, 141, 139 };
		int total = 0;
		for( int i = 0; i < 3; i++ ) {
			total = total + binC[i];
		}
		
		adaptBins( binE, binC, numWorkers);
		std::cout << "binE 1: " << binE[0] << " " << binE[1] << " " << binE[2] << " " << binE[3] << std::endl;
		
		double uniformity;
		
		uniformity = testUniformity( binC, numWorkers, total );
		
//		std::cout << "uniformity: " << uniformity << std::endl;
		
		
		// Transmit bins
	}
	else {
		// Receive bins
	}


	if (myRank == 0) {
		// Receive number of elements in each bin
	}
	else {
		// Transmit number of elements in each bin
	}


	// Change the line when the functions are written
	int isUniform = 1;

	while (isUniform == 0) {
		if (myRank == 0) {
			// Adapt bins

			// Transmit new bins

			// Receive element counts

			// Determine if uniform
			
			// Transmit isUniform update
		}
		else {
			// Receive new bins

			// Count elements

			// Receive isUniform update
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

#ifdef _TIMING_	
	auto timeBeginSwapping = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginSwapping - timeBeginBinning;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< "to identify bins" << std::endl;
#endif

	if (myRank != 0) {
		// Transmit elements to appropriate nodes

		// Receive elements from other nodes

		// Final sort

		// Export results
	}


#ifdef _TIMING_	
	auto timeEndSwapping = std::chrono::system_clock::now();
	timeElapsedSeconds = timeEndSwapping - timeBeginSwapping;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< "to swap, sort, and export data" << std::endl;
#endif


	//
	// Wrap up
	//
	
	MPI_Barrier(MPI_COMM_WORLD);


#ifdef _TIMING_	
	auto timeEnd = std::chrono::system_clock::now();
	timeElapsedSeconds = timeEnd - timeStart;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< "to run" << std::endl;
#endif


	MPI_Finalize();

	return _OKAY_;
}
