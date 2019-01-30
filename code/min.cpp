//
// sortArray
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
#include <fstream>
#include <math.h> 


//
// Local includes
//
//

//uncomment when adding to main
//#include "definitions.h"
//#include "Data.h"

//
// Function: importFiles
//



#include "min.h"
	
using namespace std;

// ****May need to make a limit for start and end point for the search for min min****
double min(double myArray[], int rows, int cols, int minThisColumn){
	double myMin = myArray[0];
	cout << "Inside find min before"<< endl;
	for (int myMin_i =0; myMin_i < rows-1 ; myMin_i++){
		if(myMin>myArray[cols*(myMin_i+1)+minThisColumn]){
			myMin = myArray[cols*(myMin_i+1)+minThisColumn];
		}
	}
	cout << "Find min completed in column: " << minThisColumn << " min: "<< myMin << endl;
	
return myMin;
}

