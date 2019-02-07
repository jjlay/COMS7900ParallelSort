//
// sortArray
//


//
// Standard includes
//

#include <string>
#include <iostream>
#include <fstream>
#include <math.h> 


//
// Function: sortArray()
//

	
using namespace std;

void sortArray(double myArray[], int rows, int cols, int sortByThisColumn) {
	//cout << "Inside Sort before"<< endl;
	double curName, curX, curY, curZ;
	int stop=1;
	//Bubble SORT HERE
	for (int iii = 0 ; iii < (rows);iii++){
		for(int jjj=0; jjj<(rows-stop); jjj++){
			if (myArray[cols*jjj+sortByThisColumn]>myArray[cols*(jjj+1)+sortByThisColumn]){
				//swap
				curName=myArray[cols*(jjj)];
				curX = myArray[cols*(jjj)+1];
				curY = myArray[cols*(jjj)+2];
				curZ = myArray[cols*(jjj)+3];
				myArray[cols*(jjj)]=myArray[cols*(jjj+1)];
				myArray[cols*(jjj)+1]=myArray[cols*(jjj+1)+1];
				myArray[cols*(jjj)+2]=myArray[cols*(jjj+1)+2];
				myArray[cols*(jjj)+3]=myArray[cols*(jjj+1)+3];
				myArray[cols*((jjj+1))]=curName;
				myArray[cols*((jjj+1))+1]=curX;
				myArray[cols*((jjj+1))+2]=curY;
				myArray[cols*((jjj+1))+3]=curZ;
			}
		}
		stop ++;	
	}
	//cout << "Sort Completed"<< endl;
	
}

