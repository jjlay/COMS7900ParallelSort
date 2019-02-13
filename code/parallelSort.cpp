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
#include "LL_sort.h"
#include "swapArrayParts.h"


using namespace std;


//
// Function: main
//

int main(int argc, char *argv[])
{
	////////////////
	//            //
	// MPI setup  //
	//            //
	////////////////

	std::string processorName;
	int myRank, numNodes;

	initializeMPI(&processorName, &myRank, &numNodes, argc, argv);

#ifdef _TIMING_
	auto timeStart = std::chrono::system_clock::now();
#endif
	
	// number of worker nodes
	int numWorkers = numNodes - 1;
	// total number of files to read
	int maxFilesToProc = numWorkers;
	// number of lines PER FILE
	int maxRows = 1000;
	//number of lines TOTAL
	unsigned int numLines = maxRows*maxFilesToProc;
	// average lines per worker node
	int avgPtsPerWorker = numLines / numWorkers;


	// set cout to print doubles' full length
//	std::cout.precision(17);
	
/*	
	std::cout << "Rank " << myRank
		<< " running on " << processorName 
		<< " with " << numNodes << " total processes" 
		<< std::endl;
*/

#ifdef _TIMING_
	auto timeBeginFilenameDistribute = std::chrono::system_clock::now();
	chrono::duration<double> timeElapsedSeconds = timeBeginFilenameDistribute - timeStart;
	cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to initialize MPI" << endl;
#endif

	//////////////////
	//              //
	// Data Import  //
	//              //
	//////////////////

	// location of the data files
	std::string homeDir = getenv("HOME");
	std::string path = homeDir + "/localstorage/public/coms7900-data/";

	std::vector<std::string> FilenameArray;

	// Retrieve the list of files to process
	if (myRank == 0)
		FilenameArray = listFiles(path, maxFilesToProc);

	// Distribute files to workers
	if (myRank == 0) {
		distributeFiles(FilenameArray, numNodes-1);
	} 
	else {
		// Receive file list
		FilenameArray = receiveFiles(myRank);
	}


	//
	// Check to make sure there is actual work
	//
	
	if (FilenameArray.size() == 0) {
		// OMG! Nothing to do!
		MPI_Finalize();
		cout << "Failed!" << endl;
		return _FAIL_;
	}

	
	MPI_Barrier(MPI_COMM_WORLD);
	
#ifdef _TIMING_
	auto timeBeginFileImport = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginFileImport - timeBeginFilenameDistribute;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to distribute the filenames" << std::endl;
#endif
	
	int sortInd = _X_;
	double myMin = 0.0;
	double myMax = 0.0;
	double *array;
	int rows = 0, cols = 0;

	if (myRank != 0) {
	        // Read data files in
	
	        array = new double[FilenameArray.size() * maxRows * _ROW_WIDTH_]; //JJL
	
	        importFiles(FilenameArray, myRank, array, &rows, &cols, maxRows);
	
	        MPI_Request tempRequest;
	        MPI_Isend(&rows, 1, MPI_INT, Rank0, mpi_Tag_RowCount, MPI_COMM_WORLD, &tempRequest);
	
	        // Perform initial sort
	        sortArray(array, rows, cols, sortInd);
	        //LL_sort(array, rows, cols, sortInd);
	}
	else {
	        // Rank 0 is going to receive the number of lines on each
	        // worker node
	
	        auto allRows = new int[numNodes];
	        allRows[0] = 0;
	        numLines = 0;
	
	        MPI_Status tempStatus;
	
	        for (auto r = 1; r < numNodes; r++) {
	                MPI_Recv(&allRows[r], 1, MPI_INT, r, mpi_Tag_RowCount,
	                        MPI_COMM_WORLD, &tempStatus);
	                numLines += allRows[r];
	        }
	
	        for (auto r = 1; r < numNodes; r++)
	                cout << "Rank " << r << " sent " << allRows[r] << " rows" << endl;
	
	        cout << "There were a total of " << numLines << " rows across all workers" << endl;
	}

	
	MPI_Barrier(MPI_COMM_WORLD);

#ifdef _TIMING_	
	auto timeBeginMinMax = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginMinMax - timeBeginFileImport;
	cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to import data" << endl;
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
		}
		
		minGlobal = allMins[1];
		maxGlobal = allMaxs[1];

		for (auto r = 2; r < numNodes; r++) {
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

	MPI_Barrier(MPI_COMM_WORLD);

	//////////////////////
	//                  //
	// Adapt Bin Sizes  //
	//                  //
	//////////////////////

	// same across all nodes
	double *binE = new double[numWorkers+1];
	// different across all nodes, master is sum of others
	int *binC = new int[numWorkers];
	
	// 2D binI for master node
	int **binI_2D = new int*[numWorkers]; // [worker][bin]
	for( int i = 0; i < numWorkers; i++ ) {
		binI_2D[i] = new int[numWorkers+1];
	}
	
	// 1D binI for workers
	int *binI_1D = new int[numWorkers+1];
	
	int result;
	MPI_Status status;
		

	// uniformity threshold
	double thresh = 0.101;
	double uniformity;
	// Change to 0 when the functions are written
	int isUniform[1];
	isUniform[0] = 1;
	
	if (myRank == 0) {
		std::cout << "ITERATION: 0" << std::endl;
		
		// Calculate initial bin edges
		getLinearBins( binE, numWorkers, myRank, minGlobal, maxGlobal );  // for real
		std::cout.precision(17);
//		std::cout << "binE: " << binE[0] << " " << binE[1] << " " << binE[2] << " " << binE[3] << std::endl;
		
		// Transmit initial bin edges
		transmitBinEdges( binE, numWorkers );
		
		// Receive initial bin counts
		receiveBinCounts( binC, numWorkers );
		std::cout << myRank << " binC: ";
		for( int i = 0; i < numWorkers; i++ ) {
			std::cout <<binC[i] << " ";
		}
		std::cout << std::endl;
		
		// Receive initial bin indices
		receiveBinIndices( binI_2D, numWorkers );
	/*
		for( int i = 0; i < numWorkers; i++ ) {
			std::cout << i+1 << " binI_2D: " << binI_2D[i][0] 
				<< " " << binI_2D[i][1] << " " 
				<< binI_2D[i][2] << " " << binI_2D[i][3] << std::endl;
		}
	*/		
		// Determine if uniform
		*isUniform = testUniformity( binC, numWorkers, thresh, &uniformity );
//	/*		
		if( *isUniform == 1 ) {
			std::cout << "Threshold:  " << thresh << std::endl;
			std::cout << "Uniformity: " << uniformity << std::endl;
			std::cout << "DONE: the bins are uniform" << std::endl;
		} else {
			std::cout << "Threshold:  " << thresh << std::endl;
			std::cout << "Uniformity: " << uniformity << std::endl;
			std::cout << "CONTINUE: the bins aren't uniform" << std::endl;
		}
//	*/	
		// Transmit isUniform update
		transmitUniformity( isUniform, numWorkers);
	} else {
		// Receive initial bin edges
		result = MPI_Recv( binE, numWorkers+1, MPI_DOUBLE, 0,
			mpi_Tag_BinEdges, MPI_COMM_WORLD, &status );
		
		binI_1D[0] = 0;
		binI_1D[numWorkers] = maxRows;
		for( int i = 1; i < numWorkers; i++ ) {
			binI_1D[i] = i*avgPtsPerWorker;
		}
		
		// get intitial bin counts, indices
		binData( array, binE, myRank, sortInd,
			numWorkers, maxRows, binI_1D, binC); // for real
//		std::cout << myRank << " binC: " << binC[0] << " " << binC[1] << " " << binC[2] << std::endl;
		
		// Transmit initial bin counts
		result = MPI_Send( binC, numWorkers, MPI_INT, 0,
			mpi_Tag_BinCounts, MPI_COMM_WORLD );
		
		// Transmit initial bin indices
		result = MPI_Send( binI_1D, numWorkers+1, MPI_INT, 0,
			mpi_Tag_BinCounts, MPI_COMM_WORLD );
		
		// Receive isUniform update
		result = MPI_Recv( isUniform, 1, MPI_INT, 0,
			mpi_Tag_isUnif, MPI_COMM_WORLD, &status );
	}
	
	int iterations = 1;
	
	while( ( *isUniform == 0 ) && (iterations < abortCount) ) {
//	while( iterations < 1 ) {
		if( myRank == 0 ) {
			cout << "ITERATION: " << iterations << endl;
			
			std::cout.precision(17);
			
			// Adapt bin edges
			adaptBins( binE, binC, numWorkers);
	/*			
			std::cout << "binE: " << binE[0] << " " 
				<< binE[1] << " " 
				<< binE[2] << " " 
				<< binE[3] << std::endl;
	*/			
			// Transmit current bin edges
			transmitBinEdges( binE, numWorkers );
			
			// Receive current bin counts
			receiveBinCounts( binC, numWorkers );
			std::cout << myRank << " binC: ";
			for( int i = 0; i < numWorkers; i++ ) {
				std::cout <<binC[i] << " ";
			}
			std::cout << std::endl;
			
			// Receive current bin indices
			receiveBinIndices( binI_2D, numWorkers );
/*
			for( int i = 0; i < numWorkers; i++ ) {
				std::cout << i+1 << " binI_2D: " << binI_2D[i][0] 
					<< " " << binI_2D[i][1] << " " 
					<< binI_2D[i][2] << " " 
					<< binI_2D[i][3] << std::endl;
			}
*/			
			// Determine if uniform
			*isUniform = testUniformity( binC, numWorkers, thresh, &uniformity );
			
			if( *isUniform == 1 ) {
				std::cout << "Threshold:  " << thresh << std::endl;
				std::cout << "Uniformity: " << uniformity << std::endl;
				std::cout << "DONE: the bins are uniform" << std::endl;
			} else {
				std::cout << "Threshold:  " << thresh << std::endl;
				std::cout << "Uniformity: " << uniformity << std::endl;
				std::cout << "CONTINUE: the bins aren't uniform" << std::endl;
			}
			
			// Transmit isUniform update
			transmitUniformity( isUniform, numWorkers);
		}
		else {
			// Receive current bin edges
			result = MPI_Recv( binE, numWorkers+1, MPI_DOUBLE, 0,
				mpi_Tag_BinEdges, MPI_COMM_WORLD, &status );
			
		//	binI_1D[0] = 0;
		//	binI_1D[numWorkers] = numLines;
		//	for( int i = 1; i < numWorkers; i++ ) {
		//		binI_1D[i] = i*avgPtsPerWorker;
		//	}
			
			// get current bin counts, indices
			binData( array, binE, myRank, sortInd,
				numWorkers, maxRows, binI_1D, binC); // for real
		//	std::cout << "binC " <<  myRank << ": " << binC[0] << " " << binC[1] << " " << binC[2] << std::endl;
			
			// Transmit current bin counts
			result = MPI_Send( binC, numWorkers, MPI_INT, 0,
				mpi_Tag_BinCounts, MPI_COMM_WORLD );
			
		//	std::cout << "binI " << myRank << ": " << binI[0] << " " << binI[1] << " " << binI[2] << " " << binI[3] << std::endl;
			// Transmit current bin indices
			result = MPI_Send( binI_1D, numWorkers+1, MPI_INT, 0,
				mpi_Tag_BinCounts, MPI_COMM_WORLD );
			
			// Receive isUniform update
			result = MPI_Recv( isUniform, 1, MPI_INT, 0,
				mpi_Tag_isUnif, MPI_COMM_WORLD, &status );
		}
		
		iterations++;
	}

	if ((iterations >= abortCount) && (myRank == Rank0)) {
		cout << "Aborted adaptBins!" << endl;
	}		

	MPI_Barrier(MPI_COMM_WORLD);

#ifdef _TIMING_	
	auto timeBeginSwapping = std::chrono::system_clock::now();
	if (myRank == Rank0) {
		timeElapsedSeconds = timeBeginSwapping - timeBeginBinning;
		std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
			<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
			<< "to identify bins" << std::endl;
	}
#endif
	
	// Broadcast binI_2D to workers
	for( int i = 0; i < numWorkers; i++ ) {
		result = MPI_Bcast( binI_2D[i], numWorkers+1, MPI_DOUBLE, 0,
			MPI_COMM_WORLD );
	}
/*
	if( myRank == 1 ) {
		for( int i = 0; i < numWorkers; i++ ) {
			std::cout << i+1 << " binI_2D: " << binI_2D[i][0] 
				<< " " << binI_2D[i][1] << " " 
				<< binI_2D[i][2] << " " 
				<< binI_2D[i][3] << std::endl;
		}
	}
*/	

	//////////////////////////////
	//                          //
	// Swap Data Between Nodes  //
	//                          //
	//////////////////////////////

	if (myRank != 0) {
		int *rowPTR;
		int *colPTR;
		double **array2;
		
		array2 = &array;
		
		// Transmit elements to appropriate nodes
		for( int fromWho = 1; fromWho <= numWorkers; fromWho++ ){
			cout << "Rank " << myRank << " is requesting data from " << fromWho << endl;
			if( fromWho != myRank ) {
				// should numRanks be numNodes or numWorkers???
				swapArrayParts( array2, rowPTR, colPTR, myRank, numWorkers, binI_2D[fromWho-1], fromWho, myRank );
			}
		}
		
		
		
		// Cleanup elements from same node
		
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

//	exportResults(array, rows, cols, numWorkers, myRank, myMin, myMax);

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
