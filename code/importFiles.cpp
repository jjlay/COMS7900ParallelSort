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

double *importFiles(std::vector<std::string> files, int myRank) {
	
	Data_COMS myData[numLines]; // compiler directive (see definitions.h)
	
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
			myData[lineCount].id = lineCount + 1;
			
			// add 1 double
			token = line.substr(14,18);
			if( isspace(token.front()) ){
				token.erase(token.begin());
			}
			myData[lineCount].data[0] = std::stod(token);
			
			// add 2 double
			token = line.substr(36,18);
			if( isspace(token.front()) ){
				token.erase(token.begin());
			}
			myData[lineCount].data[1] = std::stod(token);
			
			token = line.substr(58,18);
			if( isspace(token.front()) ){
				token.erase(token.begin());
			}
			myData[lineCount].data[2] = std::stod(token);
			
			lineCount++;
			
		}
		
		// close the file
		infile.close();
	}
	
	
	return nullptr;
}

