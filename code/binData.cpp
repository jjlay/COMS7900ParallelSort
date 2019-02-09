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
	int iter = 0;
	int rank = 3;
	
	int a = 0, b = numLines-1, c;
	
//	if( myRank == rank ) {
//		for( int i = 0; i < numPoints; i++ ) {
//			std::cout << data[4*i + sortInd] << std::endl;
//		}
//	}
	
	// loop through movable bin edges
	for( int i = 1; i < numWorkers; i++) {
		
		if( binE[i] < data[sortInd] ) {
			binI[i] = 0;
		} else if( binE[i] > data[4*(numLines-1)+sortInd] ) {
			binI[i] = numLines;
		} else {
			c = (int)ceil((a+b)/2.0);
			
			if( binE[i] < data[4*c+sortInd] )
				
			
			
			
			if(        data[4*ind+sortInd] < binE[i] and data[4*(ind+1)+sortInd] <= binE[i] ) {
				curr =  1;
			} else if( binE[i] < data[4*ind+sortInd] and binE[i] < data[4*(ind+1)+sortInd] ) {
				curr = -1;
			} else {
				curr =  0;
			}
			
			iter = 1;
		//	while( curr != 0 ) {
			while( iter < 10 ) {
				
				last = curr;
				
				if(        data[4*ind+sortInd] < binE[i] and data[4*(ind+1)+sortInd] <= binE[i] ) {
					curr =  1;
				} else if( binE[i] < data[4*ind+sortInd] and binE[i] < data[4*(ind+1)+sortInd] ) {
					curr = -1;
				} else {
					curr =  0;
				}
				
				if( curr == -last ) {
					halves++;
				}
				
				ind = ind + curr * (int)ceil(dI*pow(2.0,-halves));
				
				if( myRank == rank )
					std::cout << i << " " << ind << std::endl;
				
				iter++;
			}
			
			binI[i] = ind+1;
		}
	}
	
	for( int i = 0; i < numWorkers; i++ )
		binC[i] = binI[i+1] - binI[i];
	
}

