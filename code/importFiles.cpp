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


//
// Local includes
//
//

#include "definitions.h"
#include "Data.h"

//
// Function: importFiles
//

void importFiles(std::vector<std::string> files, int myRank,
	double *myData, int *rows, int *cols) {

	int totalLineCount = 0;

	*cols = _ROW_WIDTH_;

	//Data_COMS myData[numLines]; // compiler directive (see definitions.h)
	// std::cout << "Rank " << myRank << " before allocation" << std::endl;
	// myData = new double[maxRows * _ROW_WIDTH_]; //JJL
	// std::cout << "Rank " << myRank << " after allocation" << std::endl;
	
	// loop through files to read
	for ( auto f : files ) {

		std::cout << "=========================" << std::endl
			<< "Importing " << f << std::endl
			<< "=========================" << std::endl;

		std::ifstream infile( f );
		std::string line;
		std::string token;
		size_t pos = 0;
		int lineCount; //, tokenCount;
		
		lineCount = 0;
		// loop through lines of file
		while( std::getline(infile, line)) { // JJL and lineCount < numLines ) {
			
			// add index
			//	myData[lineCount].id = lineCount + 1;
			myData[totalLineCount * _ROW_WIDTH_ + _INDEX_] = static_cast<double>(totalLineCount+1); //JJL
			
			// ABOVE: we need to fix this so that
			// different nodes don't have duplicate indices
			
			// add 1 double
			token = line.substr(11,12);
//			if( isspace(token.front()) ){
//				token.erase(token.begin());
//			}
			//myData[lineCount].data[0] = std::stod(token);
			myData[totalLineCount * _ROW_WIDTH_ + _X_] = std::stod(token); //JJL

			// add 2 double
			token = line.substr(33,12);
//			if( isspace(token.front()) ){
//				token.erase(token.begin());
//			}
			//myData[lineCount].data[1] = std::stod(token);
			myData[totalLineCount * _ROW_WIDTH_ + _Y_] = std::stod(token); //JJL

			token = line.substr(55,11);
//			if( isspace(token.front()) ){
//				token.erase(token.begin());
//			}
			// myData[lineCount].data[2] = std::stod(token);
			myData[totalLineCount * _ROW_WIDTH_ + _Z_] = std::stod(token); //JJL

			std::cout << "Record " << std::fixed << std::setprecision(0) << totalLineCount
				<< ", Index " << myData[totalLineCount * _ROW_WIDTH_ + _INDEX_]
				<< ", X " << std::fixed << std::setprecision(5) << myData[totalLineCount * _ROW_WIDTH_ + _X_]
				<< ", Y " << myData[totalLineCount * _ROW_WIDTH_ + _Y_]
				<< ", Z " << myData[totalLineCount * _ROW_WIDTH_ + _Z_] << std::endl;

			lineCount++;
			totalLineCount++;
		}


	
		// close the file
		infile.close();
		std::cout << "Rank " << myRank << " read " << lineCount << " lines from " << f << std::endl;
	}

	std::cout << "Read " << totalLineCount << std::endl;
}

