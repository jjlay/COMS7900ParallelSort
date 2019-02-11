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


using namespace std;


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

	auto timeStart = std::chrono::system_clock::now();
	
	int numWorkers = numNodes - 1;
	int avgPtsPerWorker = numLines/numWorkers; // initial amount to move curr by
	
	// set cout to print doubles' full length
	std::cout.precision(17);
	
	
	std::cout << "Rank " << myRank
		<< " running on " << processorName 
		<< " with " << numNodes << " total processes" 
		<< std::endl;

	auto timeBeginFilenameDistribute = std::chrono::system_clock::now();
	std::chrono::duration<double> timeElapsedSeconds = timeBeginFilenameDistribute - timeStart;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(2) << timeElapsedSeconds.count() << " seconds "
		<< " to initialize MPI" << std::endl;

	// Change the following variable to the actual
	// location of the data files

	std::string homeDir = getenv("HOME");
	std::string path = homeDir + "/localstorage/public/coms7900-data/";

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


	//
	// Check to make sure there is actual work
	//
	
	if (FilenameArray.size() == 0) {
		// OMG! Nothing to do!
		MPI_Finalize();
		return _FAIL_;
	}



	auto timeBeginFileImport = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginFileImport - timeBeginFilenameDistribute;

	
	int sortInd = _X_;
	double myMin = -1 * (double) myRank; // ONLY FOR DEBUG! (JJL)
	double myMax = (double) myRank;      // ONLY FOR DEBUG! (JJL)
	double *array;
	int rows = 0, cols = 0;

	if (myRank != 0) {
		// Read data files in

		unsigned int neededMemory = FilenameArray.size() * maxRows * _ROW_WIDTH_;
		array = new double[neededMemory];
		
		importFiles(FilenameArray, myRank, array, &rows, &cols);
		
		// Perform initial sort
		timeBeginFileImport = std::chrono::system_clock::now();
		sortArray(array, rows, cols, sortInd);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);

	auto timeBeginMinMax = std::chrono::system_clock::now();
	timeElapsedSeconds = timeBeginMinMax - timeBeginFileImport;
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
		<< std::setprecision(5) << timeElapsedSeconds.count() << " seconds "
		<< " to sort data" << std::endl;



	//
	// Wrap up
	//
	


	auto timeEnd = std::chrono::system_clock::now();
	timeElapsedSeconds = timeEnd - timeStart;


	MPI_Finalize();

	return _OKAY_;
}


