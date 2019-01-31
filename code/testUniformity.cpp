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
// Function: importFiles
//

//
// SHOULD THIS FUNCTION RETURN JUST A
// MEASURE OR THE RESULT OF THE TEST?
//
double testUniformity( int *binC, int numWorkers, int total ) {
	// binC = bin counts
	
	double binN; // normalized binC
	double entropy = 0.0;
	double entropyN; // normalized entropy
	double uniformity; // final returned value
	
	for( int i = 0; i < numWorkers; i++ ) {
		
		if( binC[i] > 0) {
			binN = (1.0*binC[i]) / total;
			std::cout << binN << std::endl;
			entropy = entropy - binN*log(binN)/log(2.0);
		}
	}
	
	entropyN = entropy / (log( 1.0*numWorkers )/log(2.0));
	
	std::cout << "entropy:  " << entropy  << std::endl;
	std::cout << "entropyN: " << entropyN << std::endl;
	
	// these two asymptotically approach a max of 1, look quite similar
	uniformity = entropyN;
//	uniformity = log( (2.0 - entropyN )/2.0 ) / log(0.5);

	//  we can also use this
	//  it's linear until distribution becomes nearly completely uniform
//	uniformity = log( 1 - entropyN );
	
	return uniformity;
	
}


