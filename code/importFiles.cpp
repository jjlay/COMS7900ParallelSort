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
// Function: importFiles
//

void importFiles(std::vector<std::string> files, int myRank,
	double *myData, int rows, int cols) {
	
	//Data_COMS myData[numLines]; // compiler directive (see definitions.h)
	myData = new double[numLines]; //JJL

	// loop through files to read
	for ( auto f:files ) {
		
		std::ifstream infile( f );
		std::string line;
		std::string delimiter = "    ";
		std::string token;
		size_t pos = 0;
		int lineCount, tokenCount;
		
		lineCount = 0;
		// loop through lines of file
		while( std::getline(infile, line) and lineCount < numLines ) {
			
			// add index
			//	myData[lineCount].id = lineCount + 1;
			myData[lineCount * _ROW_WIDTH_ + _INDEX_] = static_cast<double>(lineCount+1); //JJL


			// add 1 double
			token = line.substr(14,18);
			if( isspace(token.front()) ){
				token.erase(token.begin());
			}
			//myData[lineCount].data[0] = std::stod(token);
			myData[lineCount * _ROW_WIDTH_ + _X_] = std::stod(token); //JJL

			// add 2 double
			token = line.substr(36,18);
			if( isspace(token.front()) ){
				token.erase(token.begin());
			}
			//myData[lineCount].data[1] = std::stod(token);
			myData[lineCount * _ROW_WIDTH_ + _Y_] = std::stod(token); //JJL

			token = line.substr(58,18);
			if( isspace(token.front()) ){
				token.erase(token.begin());
			}
			// myData[lineCount].data[2] = std::stod(token);
			myData[lineCount * _ROW_WIDTH_ + _Z_] = std::stod(token); //JJL

			lineCount++;
			
		}
		
		// close the file
		infile.close();
	}
}

