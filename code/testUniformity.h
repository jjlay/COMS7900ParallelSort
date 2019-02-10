#pragma once

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

int testUniformity( int *binC, int numWorkers, double thresh, double *uniformity );
	

