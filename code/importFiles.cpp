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
	
	int count = 1;
	Data_COMS data[numLines]; // compiler directive (see definitions.h)
	
	// loop through files to read
	for ( auto f:files ) {
		std::ifstream infile( f );
		std::string line;
		std::string delimiter = " ";
		std::string token;
		size_t pos = 0;
		
		// loop through lines of file
		while( std::getline(infile, line) and count <= numLines ) {
		//	std::cout << line << std::endl;
			
			while( (pos = line.find(delimiter)) != std::string::npos ) {
				
				token = line.substr(0, pos);
				std::cout << token << std::endl;
				line.erase(0, pos + delimiter.length());
				
			}
			
			
		}
		
		
		// close the file
	}
	
	
	return nullptr;
}

