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

	// FilenameArray = readListOfFile();

	if (myRank == 0) {
		// Distribute files
	} 
	else {
		// Receive file list
	}

	if (myRank != 0) {
		// Read data files in
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (myRank == 0) {
		// Receive minimums and maximums
	} 
	else {
		// Send minimums and maximums
	}





	//
	// Wrap up
	//
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return _OKAY_;
}
