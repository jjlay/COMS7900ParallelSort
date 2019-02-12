

//
// Parallel includes
//

#include "mpi.h"


//
// Standard includes
//

#include <iostream>
#include <iomanip>


//
// Local includes
//

#include "definitions.h"


//
// Function: receiveBinCounts
//
// Parameters:
//    bins - pointer to the array of bin edges
//
//    numWorkers - number of workers

void receiveBinCounts(int *binC, int numWorkers) {
	
	int *part = new int[numWorkers];
	int result;
	MPI_Status status;
	
//	std::cout << "Receiving bin counts" << std::endl;
	
	// reset binC for accumulation
	for( int i = 0; i < numWorkers; i++ ) {
		binC[i] = 0;
	}
	
	for( int i = 1; i < numWorkers+1; i++ ) {
		
	//	std::cout << "test " << 0 << std::endl;
		// add the binC's
		result = MPI_Recv( part, numWorkers, MPI_INT, i,
			mpi_Tag_BinCounts, MPI_COMM_WORLD, &status );
		
		for( int j = 0; j < numWorkers; j++ ) {
			binC[j] = binC[j] + part[j];
		}
	}
	
}


