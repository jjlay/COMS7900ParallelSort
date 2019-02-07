//
// max
//

//
// Standard includes
//

#include <iostream>
#include <math.h> 


//
// Function: max()
//

	
using namespace std;

// ****May need to make a limit for start and end point for the search for min max****
double max(double myArray[], int rows, int cols, int maxThisColumn){
	double myMax = myArray[maxThisColumn];
	int foundRow = 0;
	//cout << "Inside find max before searching "<< rows << endl;
	//for (int myMax_i =0; myMax_i < rows-1 ; myMax_i++){
	for (int myMax_i =0; myMax_i < rows ; myMax_i++){
		if(myMax<myArray[cols*(myMax_i)+maxThisColumn]){
			myMax = myArray[cols*(myMax_i)+maxThisColumn];
			foundRow = myMax_i;
		}
	}
	//cout << "Find max completed in column: " << maxThisColumn << " max: "<< myMax << " row: " << foundRow << endl;
	
return myMax;
}

