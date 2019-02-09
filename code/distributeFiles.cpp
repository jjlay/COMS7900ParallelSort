//
// distributeFiles
//


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


using namespace std;


#undef _DEBUG_


//
// Function: distributeFiles
//
// Parameters:
//   files - vector of strings containing the files
//           to be distributed to workers
//   numWorkers - the number of worker nodes not
//           counting node 0
//
// Returns:
//   nothing
//

void distributeFiles(vector<string> files, int numWorkers) {

	int currentRank = 1;
	int mpiReturn;
	MPI_Request request;


	// Pointer to the char buffer to be passed to workers
	const char *b;

	// Loop through the list of files and send them to the
	// workers in a round robin method.
	for (auto f : files) {
		// Convert the string to a char*
		b = f.c_str();

		// Send the filename asynchronously so we can keep
		// working while the worker node deals with it.
		mpiReturn = MPI_Isend(b, mpi_Max_Filename, MPI_BYTE, currentRank,
				mpi_Tag_File, MPI_COMM_WORLD, &request);


		// Loop through the worker nodes. When we reach
		// the last node, start over with rank 1.
		currentRank++;
		if (currentRank > numWorkers)
			currentRank = 1;
	}

	// Now let the nodes no that no more files are coming.
	b = "DONE!";

	for (int r = 1; r <= numWorkers; r++) {
		mpiReturn = MPI_Isend(b, mpi_Max_Filename, MPI_BYTE, r,
			mpi_Tag_File, MPI_COMM_WORLD, &request);
	}
}


