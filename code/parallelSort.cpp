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




	//
	// Wrap up
	//
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return _OKAY_;
}
