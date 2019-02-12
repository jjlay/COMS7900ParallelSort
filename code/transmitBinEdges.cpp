

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
// Function: transmitBinEdges
//
// Parameters:
//    bins - pointer to the array of bin edges
//
//    numWorkers - number of workers

void transmitBinEdges(double *binE, int numWorkers) {

	int result;
	
//	std::cout << "Transmitting bins" << std::endl;
	
	for( int i = 1; i < numWorkers+1; i++ ) {
		
		result = MPI_Send(binE, numWorkers+1, MPI_DOUBLE, i,
			mpi_Tag_BinEdges, MPI_COMM_WORLD);
		
	}
}


