//
// receiveFiles
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
#include <stdlib.h>


//
// STL includes
//

#include <vector>


//
// Local includes
//

#include "definitions.h"


//
// Function: receiveFiles
//

std::vector<std::string> receiveFiles(int myRank) {

	std::vector<std::string> files;

	int mpiReturn;
	MPI_Request request;
	MPI_Status status;

#ifdef _DEBUG_
	std::cout << "I am rank " << myRank 
		<< " max memory " << mpi_Max_Filename
		<< std::endl;
#endif

	char *buffer = static_cast<char *>(calloc(mpi_Max_Filename * 2, sizeof(char)));
	std::string strDone = "DONE!";

	int Run = 1;

	while(Run) {
		mpiReturn = MPI_Irecv(buffer, mpi_Max_Filename, MPI_BYTE,
			0, mpi_Tag_File, MPI_COMM_WORLD, &request);

		MPI_Wait(&request, &status);

		auto s = std::string(buffer);

#ifdef _DEBUG_
		std::cout << "Rank: " << myRank
			<< " :: s: " << s
			<< std::endl;
#endif


		if (s == strDone) {
			Run = 0;
		}
		else {
			files.push_back(s);
		}
	}

	return files;
}


