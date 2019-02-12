//
// importFiles
//

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


using namespace std;



//
// Function: importFiles
//

void importFiles(vector<string> files, int myRank,
	double *myData, int *rows, int *cols, int maxRows) {

	*cols = _ROW_WIDTH_;

	std::string homeDir = getenv("HOME");
	std::string prefix = homeDir + "/localstorage/public/coms7900-data/datafile";
	//const string prefix = "./data/datafile";
	const string suffix = ".txt";
	const double rowsPerFile = maxRows;
	unsigned int arrayIndex = 0;
	
	// loop through files to read
	for (auto f : files) {
		ifstream infile( f );
		string line;
		string token;
		size_t pos = 0;
		int lineCount = 0; //, tokenCount;
		
		
		//
		// Determine what the file number is
		//

		string extractedValue = f.substr(prefix.length(),
			 f.length() - prefix.length() - suffix.length());
		
		double fileIndex = stod(extractedValue) - 1;
		double totalLineCount = 1 + fileIndex * rowsPerFile;

	
		// loop through lines of file
		while( getline(infile, line) and lineCount < maxRows ) {
			
			// add index
			myData[arrayIndex * _ROW_WIDTH_ + _INDEX_] = totalLineCount;
			
			// add 1 double
			token = line.substr(13, 21);
			myData[arrayIndex * _ROW_WIDTH_ + _X_] = stod(token);

			// add 2 double
			token = line.substr(36, 21);
			myData[arrayIndex * _ROW_WIDTH_ + _Y_] = stod(token);

			token = line.substr(55, 21);
			myData[arrayIndex * _ROW_WIDTH_ + _Z_] = stod(token);

			lineCount++;
			totalLineCount++;
			arrayIndex++;
		}


	
		// close the file
		infile.close();
	}

	*rows = arrayIndex;
}

