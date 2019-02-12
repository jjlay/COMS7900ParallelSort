
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

void exportResults(int myRank, double min, double max) {

	// Display results
	std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank
		<< " has a min of " << std::setprecision(16) << min
		<< " and a max of " << std::setprecision(16) << max
		<< std::endl;

}


