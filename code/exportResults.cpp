
//
// Local includes
//

#include "definitions.h"
#include "receiveMinMax.h"
#include "transmitMinMax.h"


//
// Standard includes
//

#include <iostream>


//
// Parallel includes
//

#include "mpi.h"


//
// Function: exportResults
//
// Parameters:
//    array - pointer to the stored data
//    rows - number of rows of data
//    cols - number of columns of data
//    numBins - number of bins
//    myRank - rank of current process
//
//  Returns:
//    Nothing
//

void exportResults(double *array, int rows, int cols, int numBins,
		int myRank, double min, double max) {

#ifdef _DEBUG_
	std::cout << "exportResults" << std::endl;
#endif

	//
	// Receive results
	//

	auto mins = new double[numBins];
	auto maxs = new double[numBins];

	mins[0] = 0.0;
	maxs[0] = 0.0;

	if (myRank == Rank0) {
		// Receive min max from other ranks
		for (auto r = 1; r < numBins; r++) {
			std::cout << "Rank 0 is receiving from " << r << std::endl;
			receiveMinMax(r, &mins[r], &maxs[r]);
		}

	}
	else {
		// Send min max to rank 0
		std::cout << "Rank " << myRank << " is sending to Rank 0" << std::endl;
		transmitMinMax(min, max);
	}

	//
	// Display results
	//

	if (myRank == Rank0) {
		// Display results

		MPI_Barrier(MPI_COMM_WORLD);
	}
	else {
		MPI_Barrier(MPI_COMM_WORLD);
	}
}


