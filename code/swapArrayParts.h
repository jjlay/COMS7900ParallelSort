//
// importFiles
//

//#undef _DEBUG_

//
// STL includes
//

#include <vector>

//
//Standard Includes
//

#include <string>
#include <iostream>
#include <fstream>

//
//local includes
//

// uncomment with main
// #include "definitions.h"
// #include "Data.h"

//
//Function: swapArrayParts
//

void swapArrayParts(double *myArray[], int *rowPTR, int *colPTR, int myrank, int numranks, int *binIPTR, int fromWho, int toWho);
