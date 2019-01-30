//
// Local includes
//

#include "definitions.h"


//
// Function: transmit
//
// Parameters:
//   min - least value
//   max - greatest value
//
// Returns:
//   Nothing

void transmitMinMax(double min, double max) {

	auto minMax = new double[2];

	minMax[_MIN_] = min;
	minMax[_MAX_] = max;

	int result = MPI_Send(minMax, 2, MPI_DOUBLE, Rank0,
		mpi_Tag_SendMinMax, MPI_COMM_WORLD);
	
	char *error;
	int errorLen;
	MPI_Error_string(result, error, &errorLen);
	std::cerr << "MPI Result " << result << " : " << error << std::endl;
	

}
