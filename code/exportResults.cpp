
//
// Local includes
//

#include "definitions.h"


//
// Standard includes
//

#include <iostream>


//
// Function: exportResults
//
// Parameters:
//    array - pointer to the stored data
//    rows - number of rows of data
//    cols - number of columns of data
//    numBins - number of bins
//    myRank - rank of current process
//
//  Returns:
//    Nothing
//

void exportResults(double *array, int rows, int cols, int numBins,
		int myRank) {

#ifdef _DEBUG_
	std::cout << "exportResults" << std::endl;
#endif



}


