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
#include <iomanip>
#include <fstream>


//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"

//
// Function: importFiles
//

void adaptBins( double *binE, int *binC, int numWorkers) {
	// binE = bin edges, binC = bin counts
	
	double *temp = new double[numWorkers+1];
	double dC, dB;
	
	for( int i = 1; i < numWorkers; i++ ) {
		
		dC = ( 2.0*(binC[i] - binC[i-1]) )/( binC[i] + binC[i-1] );
		
		if( dC > 0 ) {
			dB = binE[i+1] - binE[i];
		} else if( dC < 0 ) {
			dB = -(binE[i] - binE[i-1]);
		} else {
			dB = 0;
		}
		
		temp[i] = 0.25*dC*dB;
	}
	
	for( int i = 1; i < numWorkers; i++ ) {
		binE[i] = temp[i];
	}
	
}

