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

	*cols = _ROW_WIDTH_;

	const std::string prefix = "./data/datafile";
	const std::string suffix = ".txt";
	const double rowsPerFile = maxRows;
	unsigned int arrayIndex = 0;

	// loop through files to read
	for ( auto f : files ) {

		std::cout << "=========================" << std::endl
			<< "Importing " << f << std::endl
			<< "=========================" << std::endl;

		std::ifstream infile( f );
		std::string line;
		std::string token;
		size_t pos = 0;
		int lineCount = 0; //, tokenCount;


		//
		// Determine what the file number is
		//

		std::string extractedValue = f.substr(prefix.length(),
			 f.length() - prefix.length() - suffix.length());
				
		double fileIndex = std::stod(extractedValue) - 1;
		double totalLineCount = 1 + fileIndex * rowsPerFile;

		std::cout << f << " has " << extractedValue 
			<< " which is " << std::fixed << std::setprecision(0) << fileIndex 
			<< " and starting line " << totalLineCount
			<< " with maxRows of " << rowsPerFile
			<< std::endl;

	
		// loop through lines of file
		while( std::getline(infile, line) and lineCount < numLines ) {
			
			// add index
			myData[arrayIndex * _ROW_WIDTH_ + _INDEX_] = totalLineCount;
			
			// add 1 double
			token = line.substr(13, 21);
			myData[arrayIndex * _ROW_WIDTH_ + _X_] = std::stod(token);

			// add 2 double
			token = line.substr(36, 21);
			myData[arrayIndex * _ROW_WIDTH_ + _Y_] = std::stod(token);

			token = line.substr(55, 21);
			myData[arrayIndex * _ROW_WIDTH_ + _Z_] = std::stod(token);

/*
			std::cout << "Record " << std::fixed << std::setprecision(0)
				 << totalLineCount
				<< ", Index " << myData[totalLineCount * _ROW_WIDTH_ + _INDEX_]
				<< ", X " << std::fixed << std::setprecision(5)
				 << myData[totalLineCount * _ROW_WIDTH_ + _X_]
				<< ", Y " << myData[totalLineCount * _ROW_WIDTH_ + _Y_]
				<< ", Z " << myData[totalLineCount * _ROW_WIDTH_ + _Z_]
				 << std::endl;
*/
			lineCount++;
			totalLineCount++;
			arrayIndex++;
		}


	
		// close the file
		infile.close();
		std::cout << "Rank " << myRank 
			<< " read " << lineCount 
			<< " lines from " << f 
			<< std::endl;
	}

	*rows = arrayIndex;
}

