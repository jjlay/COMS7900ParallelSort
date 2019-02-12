//
// swapArrayParts
//

//#undef _DEBUG_


//
//STL includes
//

#include <vector>

//
//Standard Includes
//

#include <sys/time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h> 
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include "mpi.h"

//
//Local includes
//
//
#include "cleanUp.h"

//#include "definitions.h"
//#include "data.h"
//

//
//Function: cleanUP 
//
using namespace std;

void cleanUp(double *pmyArray[], int *rowPTR , int *colPTR, int myrank, int numranks, int *binIPTR){
	MPI_Request request;
	MPI_Status status;
	int maxRank = numranks;
	int myRank = myrank;
	double *myArray = *pmyArray;
	double *tempArray;
	int reserveThisMuchSpace =  (4*((binIPTR[myRank+1]-binIPTR[myRank])+rowPTR[0]-binIPTR[maxRank])); 
	tempArray = (double*)malloc((reserveThisMuchSpace)*sizeof(double));
	int tempArrayCounter =0;
	for (int j = binIPTR[myRank]; j< binIPTR[myRank+1]; j++){
		for(int k =0; k< colPTR[0] ; k++){
			tempArray[tempArrayCounter] = myArray[j*4+k];
			tempArrayCounter ++;
		}
	}	
	for (int j = binIPTR[maxRank]; j< (rowPTR[0]); j++){
		for(int l =0; l< colPTR[0] ; l++){
			tempArray[tempArrayCounter] = myArray[j*4+l];
			tempArrayCounter ++; 
		}
	} 
	*pmyArray = tempArray;
	rowPTR[0]=reserveThisMuchSpace/4;
return;
}













