//
// Parallel Sort
//

//
// Standard includes
//

#include <iostream>
#include <iomanip>

//
// Parallel includes
//

#include "mpi.h"


//
// Local includes
//

#include "definitions.h"


//
// Function: main
//

int main(int argc, char *argv[])
{
	//
	// MPI setup
	//
	
	MPI_Init(&argc, &argv);

	std::cout << "Team Metropolis Parallel Sort" << std::endl;




	//
	// Wrap up
	//
	
	MPI_Finalize();

	return _OKAY_;
}
