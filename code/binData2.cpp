//
// binData2
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

#include <math.h>


//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"


//
// Function: binData2
// returns binC, updates binI
//

void binData2( double *data, double *binE, int myRank, int sortInd, 
	int numWorkers, int numPoints, int *binI, int *binC) {
	// binE: bin edges, binI: bin edge indices, binC: bin counts
	// sortInd: which column to sort by

	int currentRow = 0;

	for (auto bin = 0; bin < numWorkers; bin++) {
		binC[bin] = 0;
		binI[bin] = currentRow;

		auto stop = binE[bin+1];

		while ((data[(currentRow * 4) + sortInd] < stop) && (currentRow < numPoints)) {
			binC[bin]++;
			currentRow++;
		}
	}			
}


