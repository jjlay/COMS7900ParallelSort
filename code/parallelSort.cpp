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
// STL includes
//

#include <map>


//
// Local includes
//

#include "definitions.h"


//
// Function: main
//

int main(int argc, char *argv[])
{
	//
	// MPI setup
	//
	
	MPI_Init(&argc, &argv);

	std::cout << "Team Metropolis Parallel Sort" << std::endl;

	int numProcs, myRank, nameLen;

	auto procName = new char[MPI_MAX_PROCESSOR_NAME];

	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Get_processor_name(procName, &nameLen);

	std::cout << "Rank " << myRank << " out of " << numProcs << " nodes" 
		<< " on Hostname: " << procName << std::endl;




	//
	// Wrap up
	//
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return _OKAY_;
}
