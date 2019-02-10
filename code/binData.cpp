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
	
	int ind, done = 0;
	int iter = 0;
	int rank = 3;
	
	int a, b, c;
	
//	if( myRank == rank ) {
//		for( int i = 0; i < numPoints; i++ ) {
//			std::cout << data[4*i + sortInd] << std::endl;
//		}
//	}
	
	// loop through movable bin edges
	for( int i = 1; i < numWorkers; i++) {
		
		if( binE[i] <= data[sortInd] ) {
			binI[i] = 0;
		} else if( data[4*(numLines-1)+sortInd] <= binE[i] ) {
			binI[i] = numLines;
		} else {
			a = 0;
			b = numLines-1;
			c = (int)floor((a+b)/2.0);
			
			if( binE[i] < data[4*c+sortInd] ) {
				b = c;
			} else if( data[4*(c+1)+sortInd] <= binE[i] ) {
				a = c;
			} else if( data[4*c+sortInd] <= binE[i] and binE[i] < data[4*(c+1)+sortInd] ) {
				ind  = c;
				done = 1;
			}
			
			iter = 1;
		//	while( done != 1 ) {
			while( iter < 10 ) {
				c = (int)floor((a+b)/2.0);
				
				if( binE[i] < data[4*c+sortInd] ) {
					b = c;
				} else if( data[4*(c+1)+sortInd] <= binE[i] ) {
					a = c;
				} else if( data[4*c+sortInd] <= binE[i] and binE[i] < data[4*(c+1)+sortInd] ) {
					ind  = c;
					done = 1;
				}
				
				iter++;
			}
			
			binI[i] = ind+1;
		}
	}
	
	for( int i = 0; i < numWorkers; i++ )
		binC[i] = binI[i+1] - binI[i];
	
}

