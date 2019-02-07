
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
#include <iomanip>


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


	//
	// Receive results
	//

	auto mins = new double[numBins+1];
	auto maxs = new double[numBins+1];

	mins[0] = 0.0;
	maxs[0] = 0.0;

	if (myRank == Rank0) {
		// Receive min max from other ranks
		for (auto r = 1; r <= numBins; r++) {
			receiveMinMax(r, &mins[r], &maxs[r]);
		}

	}
	else {
		// Send min max to rank 0
		transmitMinMax(min, max);
	}

	//
	// Display results
	//

	if (myRank == Rank0) {
		// Display results
		for (auto r = 1; r <= numBins; r++)
			std::cout << "Rank " << std::fixed << std::setprecision(0) << r
				 << " has a min of " << std::setprecision(16) << mins[r]
				<< " and a max of " << std::setprecision(16) << maxs[r]
				<< std::endl;

		MPI_Barrier(MPI_COMM_WORLD);
	}
	else {
		MPI_Barrier(MPI_COMM_WORLD);
	}
}


