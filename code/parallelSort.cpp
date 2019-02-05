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
#include "transmitBinEdges.h"
#include "receiveBinCounts.h"
#include "transmitUniformity.h"


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
	
	int sortInd = _X_;
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
	
	// same across all nodes
	double *binE = new double[numWorkers+1];
	// different across all nodes, master doesn't have one
	int *binI = new int[numWorkers+1];
	// different across all nodes, master is sum of others
	int *binC = new int[numWorkers];
	
	// uniformity threshold
	double thresh = 0.10;
	// Change to 0 when the functions are written
	int isUniform[1];
	isUniform[0] = 1;
	
	// this next section requires:
	// transmitBinEdges, transmitBinCounts,
	// sortArray to have been used above,
	// transmit uniformity test
	
	
	if (myRank == 0) {
		// Calculate initial bin edges
	//	getLinearBins( binE, numWorkers, myRank, myMin, myMax );  // for real
		getLinearBins( binE, numWorkers, myRank, -1.0, 1.0 );     // for testing
		
		// Transmit initial bin edges
		transmitBinEdges( binE, numWorkers );
		
		// Receive initial bin counts
		receiveBinCounts( binC, numWorkers );
		
		// Determine if uniform
	//	isUniform = testUniformity( binC, numWorkers, numLines, thresh );
		
		// Transmit isUniform update
		transmitUniformity( isUniform, numWorkers);
	}
	else {
		int result;
		MPI_Status status;
		
		// Receive initial bin edges
		result = MPI_Recv( binE, numWorkers+1, MPI_DOUBLE, 0,
			mpi_Tag_BinEdges, MPI_COMM_WORLD, &status );
		
		// get intitial bin counts
	//	binData( array, binE, myRank, sortInd,
	//		numWorkers, numLines, binI, binC); // for real
		int binC[3] = { myRank, myRank, myRank };  // for testing
		
		// Transmit initial bin counts
		result = MPI_Send( binC, numWorkers, MPI_INT, 0,
			mpi_Tag_BinCounts, MPI_COMM_WORLD );
		
		// Receive isUniform update
		result = MPI_Recv( isUniform, 1, MPI_INT, 0,
			mpi_Tag_isUnif, MPI_COMM_WORLD, &status );
	}
	
	// this loop should look the same as the above
	/*
	while( *isUniform == 0 ) {
		if( myRank == 0 ) {
			// Adapt bin edges
			adaptBins( binE, binC, numWorkers);

			// Transmit bin edges

			// Receive bin counts

			// Determine if uniform
			isUniform = testUniformity( binC, numWorkers, total, thresh );
			
			// Transmit isUniform update
		}
		else {
			// Receive bin edges

			// get bin counts
			binData( array, binE, myRank, numWorkers, 1000, binI, binC);

			// Transmit bin counts
			
			// Receive isUniform update
		}
	}
	*/

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
