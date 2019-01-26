//
// Parallel Sort
//

//
// Standard includes
//

#include <iostream>
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

	std::cout << "Rank " << myRank
		<< " running on " << processorName 
		<< " with " << numNodes << " total processes" 
		<< std::endl;

	// Change the following variable to the actual
	// location of the data files
	std::string path = "./data/";

	auto FilenameArray = readListOfFile(path);

	if (myRank == 0) {
		// Distribute files
	} 
	else {
		// Receive file list
	}

	if (myRank != 0) {
		// Read data files in

		// Perform initial sort
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (myRank == 0) {
		// Receive minimums and maximums
	} 
	else {
		// Send minimums and maximums
	}


	if (myRank == 0) {
		// Calculate bins

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


	int isUniform = 0;

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
