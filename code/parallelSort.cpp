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

	if (myRank != 0) {
		// Read data files in
		// Data_COMS *myData;
		// myData = importFiles( FilenameArray, myRank );
		//std::cout << myData << std::endl;

		double *array = new double[FilenameArray.size() * maxRows * _ROW_WIDTH_]; //JJL
		int rows = 0, cols = 0;
		
//		std::cout << "Rank " << myRank << " is importing files" << std::endl;
		importFiles(FilenameArray, myRank, array, &rows, &cols);
//		std::cout << "rank: " << myRank << " " << array[0] << " " << array[1] << " " << array[2] << " " << array[3] << std::endl;
//		std::cout << "Rank " << myRank << " has imported files" << std::endl;
		
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

	if (myRank == 0) {
		// Receive minimums and maximums
	} 
	else {
		// Send minimums and maximums
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

	if (myRank != 0) {
		// Transmit elements to appropriate nodes

		// Receive elements from other nodes

		// Final sort

		// Export results
	}



	//
	// Wrap up
	//
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return _OKAY_;
}
