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

#include <math.h>


//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"

//
// Function: binData
// returns binC, updates binI
//

void binData( double *data, double *binE, int myRank, int sortInd, int numWorkers, int numPoints, int *binI, int *binC) {
	// binE: bin edges, binI: bin edge indices, binC: bin counts
	// sortInd: which column to sort by
	
	int test, halves, curr, last, ind;
	int dI = numPoints/numWorkers; // initial amount to move curr by
	
	// match with Wallin's 1 indexing
	binI[0] = 0;
	binI[numWorkers] = numPoints;
	
	/*
	std::cout << std::endl;
	std::cout << "Binning......" << std::endl;
	std::cout << "binE: " << binE[0] << " " << binE[1] << " " << binE[2] << " " << binE[3] << std::endl;
	std::cout << std::endl;
	*/
	
	// loop through movable bin edges
	for( int i = 1; i < numWorkers; i++) {
		
		// will be fed back into binI[i] later
		ind = binI[i];
		// don't bisect initially
		halves = 0;
		
		if(        data[4*ind+sortInd] < binE[i] and data[4*ind+sortInd+1] <= binE[i] ) {
			curr =  1;
		} else if( binE[i] < data[4*ind+sortInd] and binE[i] < data[4*ind+sortInd+1] ) {
			curr = -1;
		} else {
			curr =  0;
		}
		
		/*
		std::cout << "ind: " << ind << std::endl;
		std::cout << data[ind] << " " << binE[i] << " " << data[ind+1] << std::endl;
		std::cout << "curr:  " << curr << std::endl;
		std::cout << std::endl;
		*/
		
		if( curr != 0 )
			ind = ind + curr * (int)ceil(dI*pow(2.0,-halves));
		
		while( curr != 0 ) {
			
			/*
			std::cout << "ind: " << ind << std::endl;
			std::cout << data[ind] << " " << binE[i] << " " << data[ind+1] << std::endl;
			std::cout << "curr:  " << curr << std::endl;
			*/
			
			last = curr;
			
			if(        data[4*ind+sortInd] < binE[i] and data[4*ind+sortInd+1] <= binE[i] ) {
				curr =  1;
			} else if( binE[i] < data[4*ind+sortInd] and binE[i] < data[4*ind+sortInd+1] ) {
				curr = -1;
			} else {
				curr =  0;
			}
			
			if( curr == -last ) {
				halves++;
			}
			
			ind = ind + curr * (int)ceil(dI*pow(2.0,-halves));
			
			std::cout << std::endl;
		}
		
		binI[i] = ind+1;
	}
	
	for( int i = 0; i < numWorkers; i++ )
		binC[i] = binI[i+1] - binI[i];
	
}

