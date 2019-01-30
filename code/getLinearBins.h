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


//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"


//
// Function: getLinearBins
//

void getLinearBins( double *binE, int numNodes, int myRank,
	double minGlobal, double maxGlobal );


