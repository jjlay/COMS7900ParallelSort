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
#include "receiveBinIndices.h"
#include "sortArray.h"
#include "exportResults.h"
#include "min.h"
#include "max.h"


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
	int avgPtsPerWorker = numLines/numWorkers; // initial amount to move curr by
	
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
	double myMin = -1 * (double) myRank; // ONLY FOR DEBUG! (JJL)
	double myMax = (double) myRank;      // ONLY FOR DEBUG! (JJL)
	double *array;
	int rows = 0, cols = 0;

	if (myRank != 0) {
		// Read data files in

		array = new double[FilenameArray.size() * maxRows * _ROW_WIDTH_]; //JJL
		
		importFiles(FilenameArray, myRank, array, &rows, &cols);
	//	std::cout << "Rows: " << rows << ", Cols: " << cols << std::endl;
		
		// Perform initial sort
		sortArray(array, rows, cols, sortInd);
		
	//	std::cout << "rank " << myRank << ": " << array[cols*0+sortInd] << " " << array[cols*999+sortInd] << std::endl;
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
	double minGlobal, maxGlobal;

	if (myRank == 0) {
		// Receive minimums and maximums
		allMins[Rank0] = 0.0;
		allMaxs[Rank0] = 0.0;

		for (auto r = 1; r < numNodes; r++) {
			receiveMinMax(r, &allMins[r], &allMaxs[r]);
			//std::cout << "Rank " << r << " sent " << allMins[r]
			//	<< " and " << allMaxs[r] << std::endl;
		}
		
	//	std::cout.precision(17);
		minGlobal = allMins[1];
		maxGlobal = allMaxs[1];
	//	std::cout << allMins[1] << " " << allMaxs[1] << std::endl;
		for (auto r = 2; r < numNodes; r++) {
			std::cout << allMins[r] << " " << allMaxs[r] << std::endl;
			if( minGlobal > allMins[r] )
				minGlobal = allMins[r];
			if( maxGlobal < allMaxs[r] )
				maxGlobal = allMaxs[r];
		}
	//	std::cout << minGlobal << " " << maxGlobal << std::endl;
	} 
	else {
		// Send minimums and maximums
		myMin = min(array, rows, cols, _X_);
		myMax = max(array, rows, cols, _X_);
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
	// different across all nodes, master is sum of others
	int *binC = new int[numWorkers];
	
	// uniformity threshold
	double thresh = 0.10;
	// Change to 0 when the functions are written
	int isUniform[1];
	isUniform[0] = 1;
	
	if (myRank == 0) {
		// 2D binI for master node
		int **binI = new int*[numWorkers]; // [worker][bin]
		for( int i = 0; i < numWorkers; i++ ) {
			binI[i] = new int[numWorkers+1];
		}
		
	//	std::cout << "min: " << minGlobal << std::endl;
	//	std::cout << "max: " << maxGlobal << std::endl;
		
		// Calculate initial bin edges
		getLinearBins( binE, numWorkers, myRank, minGlobal, maxGlobal );  // for real
		std::cout.precision(17);
		std::cout << "binE: " << binE[0] << " " << binE[1] << " " << binE[2] << " " << binE[3] << std::endl;
		
		// Transmit initial bin edges
		transmitBinEdges( binE, numWorkers );
		
		// Receive initial bin counts
		receiveBinCounts( binC, numWorkers );
		std::cout << "binC " <<  myRank << ": " << binC[0] << " " << binC[1] << " " << binC[2] << std::endl;
		
		// Receive initial bin counts
		receiveBinIndices( binI, numWorkers );
		for( int i = 0; i < numWorkers; i++ ) {
			std::cout << i << " binI: " << binI[i][0] << " " << binI[i][1] << " " << binI[i][2] << " " << binI[i][3] << std::endl;
		}
		
		// Determine if uniform
		*isUniform = testUniformity( binC, numWorkers, numLines, thresh );
		
		// Transmit isUniform update
		transmitUniformity( isUniform, numWorkers);
	} else {
		int *binI = new int[numWorkers+1];
		int result;
		MPI_Status status;
		
		// Receive initial bin edges
		result = MPI_Recv( binE, numWorkers+1, MPI_DOUBLE, 0,
			mpi_Tag_BinEdges, MPI_COMM_WORLD, &status );
		
		binI[0] = 0;
		binI[numWorkers] = numLines;
		for( int i = 1; i < numWorkers; i++ ) {
			binI[i] = i*avgPtsPerWorker;
		}
		
		// get intitial bin counts, indices
		binData( array, binE, myRank, sortInd,
			numWorkers, numLines, binI, binC); // for real
		std::cout << "binC " <<  myRank << ": " << binC[0] << " " << binC[1] << " " << binC[2] << std::endl;
		
		// Transmit initial bin counts
		result = MPI_Send( binC, numWorkers, MPI_INT, 0,
			mpi_Tag_BinCounts, MPI_COMM_WORLD );
		
	//	std::cout << "binI " << myRank << ": " << binI[0] << " " << binI[1] << " " << binI[2] << " " << binI[3] << std::endl;
		// Transmit initial bin indices
		result = MPI_Send( binI, numWorkers+1, MPI_INT, 0,
			mpi_Tag_BinCounts, MPI_COMM_WORLD );
		
		// Receive isUniform update
		result = MPI_Recv( isUniform, 1, MPI_INT, 0,
			mpi_Tag_isUnif, MPI_COMM_WORLD, &status );
		
		if( *isUniform == 1 ) {
			std::cout << "DONE: the bins are uniform" << std::endl;
		} else {
			std::cout << "CONTINUE: the bins aren't uniform" << std::endl;
		}
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

	exportResults(array, rows, cols, numWorkers, myRank, myMin, myMax);

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
