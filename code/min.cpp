//
// min
//


//
// Standard includes
//

#include <iostream>
#include <math.h> 


//
// Function: min()
//

using namespace std;

// ****May need to make a limit for start and end point for the search for min min****
double min(double myArray[], int rows, int cols, int minThisColumn){
	double myMin = myArray[minThisColumn];
	//cout << "Inside find min before"<< endl;
	for (int myMin_i =1; myMin_i < rows ; myMin_i++){
		if(myMin>myArray[cols*(myMin_i)+minThisColumn]){
			myMin = myArray[cols*(myMin_i)+minThisColumn];
		}
	}
	//cout << "Find min completed in column: " << minThisColumn << " min: "<< myMin << endl;
	
return myMin;
}

