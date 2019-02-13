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
#include "swapArrayParts.h"

//#include "definitions.h"
//#include "data.h"
//

//
//Function: swapArrayParts
//

using namespace std;

struct Node{
	double ll_location;
	double ll_x;
	double ll_y;
	double ll_z;
	struct Node *next;
};

void swapArrayParts(double *pmyArray[], int *rowPTR , int *colPTR, int myrank, int numranks, int *binIPTR, int fromWho, int toWho){
	MPI_Request request;
	MPI_Status status;

	int maxRank = numranks;
	int myRank = myrank;
	int *myBinI;
	myBinI  =(int*) malloc((maxRank+1)*sizeof(int));
	int *yourBinI;
	yourBinI = (int*) malloc((maxRank+1)*sizeof(int));
	int *storedBinIstart;
	storedBinIstart = (int*)malloc((maxRank+1)*sizeof(int));
	int *storedBinIend;
	storedBinIend = (int*)malloc((maxRank+1)*sizeof(int));
	int myStartRow = 0;
	int myEndRow = 0;
	int myArrayLength = colPTR[0]*rowPTR[0];
	int myAmountToReceive;
	int myAmountToSend;
	int mySendStartingPoint;
	double *myArray = *pmyArray;

	if(fromWho == myRank) {
		for(int mi =0; mi<maxRank+1; mi++){
			myBinI[mi] = binIPTR[mi];
		}

		cout << "Rank " << myRank << " is sending data to " << toWho << endl;
		MPI_Isend(myBinI, (maxRank+1), MPI_INT, toWho,999, MPI_COMM_WORLD, &request);
	}

	if(myRank == toWho) {
		cout << "Rank " << myRank << " is waiting for " << fromWho << " to send it data" << endl;
		MPI_Recv(yourBinI, (maxRank+1), MPI_INT, fromWho , 999, MPI_COMM_WORLD, &status);
		myStartRow = yourBinI[myRank];
		myEndRow = yourBinI[myrank+1];
		storedBinIstart[fromWho+1] = myStartRow;
		storedBinIend[fromWho+1] = myEndRow;
		for (int itest = 1; itest< maxRank+1; itest++){
		}
	}

	cout << "Rank " << myRank << " has reached the first barrier in swapArray" << endl;
	MPI_Barrier(MPI_COMM_WORLD);

	if(myRank ==fromWho){
		myAmountToSend = 4*(myBinI[toWho+1]-myBinI[toWho]);
		mySendStartingPoint = 4*(myBinI[toWho]);
		MPI_Isend(&myArray[mySendStartingPoint], myAmountToSend, MPI_DOUBLE, toWho, 888,  MPI_COMM_WORLD, &request); 
	}
	
	cout << "Rank " << myRank << " has reached the second barrier in swapArray" << endl;
	MPI_Barrier(MPI_COMM_WORLD);

	if(myRank == toWho){
		myAmountToReceive = 4*(myEndRow-myStartRow);
		double *receiveThis;
		receiveThis = (double*) malloc((myAmountToReceive)*sizeof(double));
		MPI_Recv(receiveThis, myAmountToReceive, MPI_DOUBLE, fromWho, 888 , MPI_COMM_WORLD,&status);
		double *tempArray;
		tempArray = (double*) malloc(((rowPTR[0]*4)+(myAmountToReceive))*sizeof(double));
		for(int fill = 0; fill< (rowPTR[0]*4);fill++){
			tempArray[fill] = myArray[fill];
		}
		for(int fill2 = (rowPTR[0]*4); fill2 < (rowPTR[0]*4)+myAmountToReceive; fill2++){
			tempArray[fill2] = receiveThis[fill2-rowPTR[0]*4];
		}
		*pmyArray = tempArray;

		rowPTR[0]= rowPTR[0]+ myAmountToReceive/4;
	}

	cout << "Rank " << myRank << " has reached the last barrier in swapArray" << endl;
	MPI_Barrier(MPI_COMM_WORLD);


return;
}













