//
// importFiles
//


#undef _DEBUG_


//
// STL includes
//

#include <vector>

#include <math.h>

//
// Standard includes
//

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>



//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"

//
// Function: testUniformity
//

int testUniformity( int *binC, int numWorkers, int total, double thresh, double *uniformity ) {
	// binC = bin counts
	
	int isUniform; 	// final returned value
	double avgPts = (1.0*total)/numWorkers;
	double maxPts = (double) binC[0];
	double minPts = (double) binC[0];
	
	for( int i = 1; i < numWorkers; i++ ) {
		if( binC[i] > maxPts) {
			maxPts = (double) binC[i];
		}
		if( binC[i] < minPts) {
			minPts = (double) binC[i];
		}

	}
	
	// the actual equations
	*uniformity = (maxPts - avgPts)/avgPts + (avgPts - minPts)/avgPts;
	*uniformity = *uniformity/2;
	
	if( *uniformity < thresh ){
		isUniform = 1;
	} else {
		isUniform = 0;
	}
	
	return isUniform;
	
}


