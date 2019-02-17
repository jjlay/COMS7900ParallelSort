//
// testSort
//


//
// Standard includes
//

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;


//
// Parallel includes
//

#include "mpi.h"


//
// Local includes
// 

#include "definitions.h"


//
// Function: testArray()
//

	
using namespace std;

int testSort(double myArray[], int rows, int cols, int sortByThisColumn) {

	double previous = myArray[sortByThisColumn];

	for (auto r = 1; r < rows; r++) {
		auto current = myArray[(r * cols) + sortByThisColumn];
		if (current < previous) {
			cout << "OMG! Data is not sorted!" << endl
				<< "previous = " << fixed << setprecision(17) << previous << endl
				<< "current = " << current << endl;
			MPI_Abort(MPI_COMM_WORLD, _FAIL_);
		}
		else {
			previous = current;
		}
	}

	cout << "All " << rows << " rows of data is sorted" << endl;
	return _OKAY_;
}

