//
// importFiles
//


#undef _DEBUG_


//
// STL includes
//

#include <vector>

//
// Standard includes
//

#include <string>
#include <iostream>
#include <fstream>


//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"

//
// Function: getLinearBins
//

void getLinearBins( double *binE, int numWorkers, int myRank,
	double minGlobal, double maxGlobal ) {
	
	for( int i = 0; i < numWorkers+1; i++ ) {
		
		binE[i] = minGlobal + (maxGlobal - minGlobal)*( (1.0*i)/numWorkers );
		// print for debug
	//	std::cout << "linear bin edge " << i << ": " << binE[i] << std::endl;
		
	}
	
}

