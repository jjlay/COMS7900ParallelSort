#pragma once

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

#include <math.h>

//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"


//
// Function: binDAta
//

void binData( double *data, double *binE, int myRank, int numWorkers, int numPoints, int *binI, int *binC);


