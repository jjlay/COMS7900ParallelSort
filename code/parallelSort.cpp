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
#include "adaptBins.h"
#include "testUniformity.h"
#include "binData.h"
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
		distributeFiles(FilenameArray, numNodes-1);
	} 
	else {
		// Receive file list
		FilenameArray = receiveFiles(myRank);
	}

#ifdef _TIMING_
	auto timeBeginFileImport = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginFileImport - timeBeginFilenameDistribute;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to distribute the filenames" << std::endl;

#endif

	double myMin = -1 * (double) myRank; 
	double myMax = (double) myRank;
	double *array;

	if (myRank != 0) {
		// Read data files in
		// Data_COMS *myData;
		// myData = importFiles( FilenameArray, myRank );
		//std::cout << myData << std::endl;

		array = new double[FilenameArray.size() * maxRows * _ROW_WIDTH_]; //JJL
		int rows = 0, cols = 0;
		
		importFiles(FilenameArray, myRank, array, &rows, &cols);
		
		// Perform initial sort
	}
	
	MPI_Barrier(MPI_COMM_WORLD);

#ifdef _TIMING_	
	auto timeBeginMinMax = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginMinMax - timeBeginFileImport;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to import data" << std::endl;
#endif

	auto allMins = new double[numNodes];
	auto allMaxs = new double[numNodes];

	if (myRank == 0) {
		// Receive minimums and maximums
		allMins[Rank0] = 0.0;
		allMaxs[Rank0] = 0.0;

		for (auto r = 1; r < numNodes; r++) {
			receiveMinMax(r, &allMins[r], &allMaxs[r]);
			std::cout << "Rank " << r << " sent " << allMins[r]
				<< " and " << allMaxs[r] << std::endl;
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

	int *binE = new int[numWorkers+1];
	int *binI = new int[numWorkers+1];
	int *binC = new int[numWorkers];
	
	if (myRank == 0) {
		// Calculate initial linear bins
		getLinearBins( binE, numWorkers, myRank, minGlobal, maxGlobal );
		
		// Transmit bin edges
	//	transmitBinEdges();
	}
	else {
		// Receive bin edges
	}


	if (myRank == 0) {
		// Receive number of elements in each bin
	}
	else {
		// bin the data
		binData( array, binE, myRank, numWorkers, 1000, binI, binC);
		
		// Transmit number of elements in each bin
	//	transmitBinCounts();
	}

	// uniformity threshold
	double thresh = 0.10;
	// Change to 0 when the functions are written
	int isUniform = 1;

	while (isUniform == 0) {
		if (myRank == 0) {
			// Adapt bins
			adaptBins( binE, binC, numWorkers);

			// Transmit new bins

			// Receive element counts

			// Determine if uniform
			isUniform = testUniformity( binC, numWorkers, total, thresh );
			
			// Transmit isUniform update
		}
		else {
			// Receive new bins

			// Count elements
			binData( array, binE, myRank, numWorkers, 1000, binI, binC);

			// Transmit element counts
			
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
