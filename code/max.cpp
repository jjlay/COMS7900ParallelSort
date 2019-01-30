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



#include "max.h"
	
using namespace std;

// ****May need to make a limit for start and end point for the search for min max****
double max(double myArray[], int rows, int cols, int maxThisColumn){
	double myMax = myArray[0];
	cout << "Inside find max before"<< endl;
	for (int myMax_i =0; myMax_i < rows-1 ; myMax_i++){
		if(myMax<myArray[cols*(myMax_i+1)+maxThisColumn]){
			myMax = myArray[cols*(myMax_i+1)+maxThisColumn];
		}
	}
	cout << "Find max completed in column: " << maxThisColumn << " max: "<< myMax << endl;
	
return myMax;
}

