//
// Parts Forecasting
//

//
// Standard includes
//

#include <iostream>
#include <iomanip>
#include <string>


//
// Parallel includes
//

#include "mpi.h"


//
// STL includes
//

#include <map>


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

	std::cout << "Parts Forecasting" << std::endl;

	int numProcs, myRank, nameLen;

	auto procName = new char[MPI_MAX_PROCESSOR_NAME];

	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Get_processor_name(procName, &nameLen);

	std::cout << "Rank " << myRank << " out of " << numProcs << " nodes" 
		<< " on Hostname: " << procName << std::endl;

	std::map<int, std::string> nodeList;
	std::map<std::string, int> nameCount;
	auto nodeState = new int[numProcs];
	int myState = NodeUndefined;


	if (myRank == 0) {
		std::string s(procName);
		nodeList[0] = s;

		// We need to collect the nodes
		auto tempName = new char[MPI_MAX_PROCESSOR_NAME];
		MPI_Status tempStatus;

		for (auto n = 1; n < numProcs; n++) {
			MPI_Recv(tempName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, n, mpi_Tag_Name, MPI_COMM_WORLD, &tempStatus);
			s.assign(tempName);
			nodeList[n] = s;
		}
	} else {
		// Send my node information
		MPI_Send(procName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, mpi_Tag_Name, MPI_COMM_WORLD);
	}

	// Decide which nodes we will let work and which ones will exit
	if (myRank == 0) {
		for (auto n = 0; n < numProcs; n++)
			nodeState[n] = NodeUndefined;

		for (auto n = 0; n < numProcs; n++) {
			std::cout << "Node " <<  n << " : " << nodeList[n] << std::endl;

			if (nameCount.find(nodeList[n]) == nameCount.end()) {
				// This node has not been seen yet
				nameCount[nodeList[n]] = 1;

				nodeState[n] = 0;
			} else {
				nameCount[nodeList[n]]++;

				if (nameCount[nodeList[n]] == 2)
					nodeState[n] = 1;
				else 
					nodeState[n] = NodeExit;
			}
		}

		for (auto n : nameCount)
			std::cout << n.first << " : " << n.second << std::endl;

		for (auto n = 0; n < numProcs; n++) {
			std::cout << "Node " << n << " state " << nodeState[n] << std::endl;
			MPI_Send(&nodeState[n], 1, MPI_INT, n, mpi_Tag_State, MPI_COMM_WORLD);
		}
	} 
	else {
		MPI_Status tempStatus;
		MPI_Recv(&myState, 1, MPI_INT, 0, mpi_Tag_State, MPI_COMM_WORLD, &tempStatus);

		std::cout << "Node " << myRank << " and my state is " << myState << std::endl;
	}


	//
	// Wrap up
	//
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return _OKAY_;
}
