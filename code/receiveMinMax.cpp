//
// Local includes
//

#include "definitions.h"


//
// Standard includes
//

#include <iostream>


//
// Parallel includes
//

#include "mpi.h"


//
// Function: receiveMinMax
//
// Parameters:
//   min - least value
//   max - greatest value
//
// Returns:
//   Nothing

void receiveMinMax(int sourceRank, double *min, double *max) {

	MPI_Status status;
	auto minMax = new double[2];

	minMax[_MIN_] = 0.0;
	minMax[_MAX_] = 0.0;

	int result = MPI_Recv(minMax, 2, MPI_DOUBLE, sourceRank,
		mpi_Tag_SendMinMax, MPI_COMM_WORLD, &status);

	int counter;
	MPI_Get_count(&status, MPI_DOUBLE, &counter);

	min[0] = minMax[_MIN_];
	max[0] = minMax[_MAX_];
}


