//
// distributeFiles
//


#define _DEBUG_


//
// Parallel includes
//

#include "mpi.h"


//
// Standard includes
//

#include <string>
#include <iostream>


//
// STL includes
//

#include <vector>


//
// Local includes
//

#include "definitions.h"


//
// Function: distributeFiles
//

void distributeFiles(std::vector<std::string> files, int numWorkers) {

	int currentRank = 1;
	int mpiReturn;
	MPI_Request request;

#ifdef _DEBUG_
	std::cout << "There are " << numWorkers << " workers" << std::endl;
#endif

	const char *b;

	for (auto f : files) {
		b = f.c_str();

	mpiReturn = MPI_Isend(b, mpi_Max_Filename, MPI_BYTE, currentRank,
		mpi_Tag_File, MPI_COMM_WORLD, &request);

#ifdef _DEBUG_
		std::cout << "Dest: " << currentRank
			<< " :: f: " << f
			<< " :: b: " << b 
			<< " :: len: " << f.size() 
			<< std::endl;
#endif

		currentRank++;
		if (currentRank >= numWorkers)
			currentRank = 1;
	}

}


