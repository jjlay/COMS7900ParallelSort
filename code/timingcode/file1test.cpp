
#include "../importFiles.h"
#include "../listFiles.h"
#include "../distributeFiles.h"
#include "../receiveFiles.h"
#include "../definitions.h"
#include "../initializeMPI.h"


#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>


using namespace std;


int main(int argc, char *argv[]) {

	
	//
	// MPI setup
	//

	std::string processorName;
	int myRank, numNodes;

	initializeMPI(&processorName, &myRank, &numNodes, argc, argv);

	cout << "Distribute/Receive Files Timing" << std::endl;

	auto timeStart = std::chrono::system_clock::now();

	
	int numWorkers = numNodes - 1;
	int avgPtsPerWorker = numLines/numWorkers; // initial amount to move curr by
	
	// set cout to print doubles' full length
	std::cout.precision(17);
	
	
	std::cout << "Rank " << myRank
		<< " running on " << processorName 
		<< " with " << numNodes << " total processes" 
		<< std::endl;



	// Change the following variable to the actual
	// location of the data files

	std::string homeDir = getenv("HOME");
	std::string path = homeDir + "/localstorage/public/coms7900-data/";

	std::cout << "Data path: " << path << std::endl;

	std::vector<std::string> FilenameArray;

	if (myRank == 0)
		FilenameArray = listFiles(path);

	if (myRank == 0) {
		// Distribute files
		distributeFiles(FilenameArray, numNodes-1);
	} 
	else {
		// Receive file list
		FilenameArray = receiveFiles(myRank);
	}


	auto timeBeginFileImport = std::chrono::system_clock::now();
	std::chrono::duration<double> timeElapsedSeconds = timeBeginFileImport - timeStart;

	if (myRank == 0) {
		std::cout << "Rank " << std::fixed << std::setprecision(0) << myRank << " took "
			<< std::setprecision(4) << timeElapsedSeconds.count() << " seconds "
			<< " to distribute the filenames" << std::endl;
	}
	MPI_Finalize();

	return _OKAY_;
}

