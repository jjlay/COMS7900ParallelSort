//
// Initialize MPI
//

//
// Standard includes
//

#include <iostream>
#include <string>


//
// Parallel includes
//

#include "mpi.h"


//
// Local includes
//

#include "definitions.h"


//
// Function: initializeMPI
//

void initializeMPI(std::string *processorName, int *myRank, int *numNodes, int argc, char *argv[])
{
	//
	// MPI setup
	//
	
	MPI_Init(&argc, &argv);


	auto localProcName = new char[MPI_MAX_PROCESSOR_NAME];
	int localNumProcs, localMyRank, localNameLen;

	MPI_Comm_size(MPI_COMM_WORLD, &localNumProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &localMyRank);
	MPI_Get_processor_name(localProcName, &localNameLen);

	*myRank = localMyRank;
	*numNodes = localNumProcs;
	*processorName = std::string(localProcName);

}
