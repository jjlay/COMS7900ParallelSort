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

void binData( double *data, double *binE, int myRank, int sortInd, 
	int numWorkers, int numPoints, int *binI, int *binC) {
	// binE: bin edges, binI: bin edge indices, binC: bin counts
	// sortInd: which column to sort by
	
	int ind, done;
	int iter = 0;
	int rank = 3;
	
	int a, b, c;
	
	std::cout.precision(17);
	/*
	if( myRank == rank ) {
	std::cout << "maxRows " << numPoints << std::endl;
	int count = 0;
	if( myRank == rank ) {
		for( int i = 0; i < numPoints; i++ ) {
			if( binE[1] < data[4*i + sortInd] and data[4*i + sortInd] < binE[2] ) {
				count++;
				std::cout << count << " " << data[4*i + sortInd] << std::endl;
			}
		}
	}
	}
	*/
		
	// loop through movable bin edges
	for( int i = 1; i < numWorkers; i++) {
		
		if( binE[i] <= data[sortInd] ) {
			binI[i] = 0;
		} else if( data[4*(numPoints-1)+sortInd] <= binE[i] ) {
			binI[i] = numPoints;
		} else {
			done = 0;
			a = binI[i-1]+1;
			a = 0;
			b = numPoints-1;
			c = (int)floor((a+b)/2.0);
			
//			if( myRank == rank )
//				std::cout << "acb " << a << " " << c << " " << b << std::endl;
			
			if( binE[i] < data[4*c+sortInd] ) {
				b = c;
			} else if( data[4*(c+1)+sortInd] <= binE[i] ) {
				a = c;
			} else if( data[4*c+sortInd] <= binE[i] and binE[i] < data[4*(c+1)+sortInd] ) {
				ind  = c;
				done = 1;
//				if( myRank == rank )
//					std::cout << "done" << std::endl;
			}
			
			iter = 1;
		//	while( done != 1 ) {
			while( done != 1 and iter < abortCount ) {
		//	while( iter < 10 ) {
				c = (int)floor((a+b)/2.0);
				
//				if( myRank == rank )
//					std::cout << "acb " << a << " " << c << " " << b << std::endl;
				
				if( binE[i] < data[4*c+sortInd] ) {
					b = c;
				} else if( data[4*(c+1)+sortInd] <= binE[i] ) {
					a = c;
				} else if( data[4*c+sortInd] <= binE[i] and binE[i] < data[4*(c+1)+sortInd] ) {
					ind  = c;
					done = 1;
//				if( myRank == rank )
//					std::cout << "done" << std::endl;
				}
				
				iter++;
			}
			
			binI[i] = ind+1;
		}
//		if( myRank == rank )
//			std::cout << "test: " << data[4*ind+sortInd] << " " << binE[i] << " " << data[4*(ind+1)+sortInd] << std::endl;
	}
	
	for( int i = 0; i < numWorkers; i++ )
		binC[i] = binI[i+1] - binI[i];
	
}

