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

	int result = MPI_Send(
	

}
