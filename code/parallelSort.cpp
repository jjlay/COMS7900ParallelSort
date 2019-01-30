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
	
	
	// set cout to print doubles' full length
	std::cout.precision(17);
	
	
	std::cout << "Rank " << myRank
		<< " running on " << processorName 
		<< " with " << numNodes << " total processes" 
		<< std::endl;

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


	double *array;
	int rows = 0, cols = 0;

	if (myRank != 0) {
		// Read data files in
		// Data_COMS *myData;
		// myData = importFiles( FilenameArray, myRank );
		//std::cout << myData << std::endl;

		std::cout << "Rank " << myRank << " is importing files" << std::endl;
		importFiles(FilenameArray, myRank, array, &rows, &cols);
		std::cout << "Rank " << myRank << " has imported files" << std::endl;
		
		// Perform initial sort
	}
	
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	
	if (myRank == 0) {
		// Receive minimums and maximums
	} 
	else {
		// Send minimums and maximums
	}
	
	
	double *binE;
	double minGlobal = -1.0, maxGlobal = 1.0; // remove -1,+1 when done testing

	if (myRank == 0) {
		// Calculate bins
		getLinearBins( binE, numNodes-1, myRank, minGlobal, maxGlobal );

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
