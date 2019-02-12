

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
// Function: receiveBinIndices
//
// Parameters:
//    bins - pointer to the array of bin edges
//
//    numWorkers - number of workers

void receiveBinIndices(int **binI, int numWorkers) {
	
	int  *part = new int[numWorkers+1]; 		// [bin]
	int result;
	MPI_Status status;
	
//	std::cout << "Receiving bin indices" << std::endl;
	
	for( int i = 1; i < numWorkers+1; i++ ) {
		
		// add the binC's
		result = MPI_Recv( part, numWorkers+1, MPI_INT, i,
			mpi_Tag_BinCounts, MPI_COMM_WORLD, &status );
		
		for( int j = 0; j < numWorkers+1; j++ ) {
			binI[i-1][j] = part[j];
		}
	}
	
}


