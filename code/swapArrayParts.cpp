#include <vector>
#include <sys/time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h> 
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include "mpi.h"

#include "swapArrayParts.h"
	
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
		cout << "Rank " << myRank << " ";
		cout << "myArray at the beginning" << endl;
		for (int i = 0; i <*rowPTR; i++) {
			cout << "Row " << i << " :";
			for (int j = 0; j < *colPTR; j++)
				cout << myArray[i * 4 + j] << " : ";
			cout << std::endl;
		}

		for(int mi =0; mi<maxRank+1; mi++){
			myBinI[mi] = binIPTR[mi];
		}	
		MPI_Isend(myBinI, (maxRank+1), MPI_INT, toWho,999, MPI_COMM_WORLD, &request);
		cout << "Rank: " << myRank << " sent binI to: " << toWho << endl;
	}
	if(myRank ==toWho){
		MPI_Recv(yourBinI, (maxRank+1), MPI_INT, fromWho , 999, MPI_COMM_WORLD, &status);
		myStartRow = yourBinI[myRank];
		myEndRow = yourBinI[myrank+1];
		storedBinIstart[fromWho+1] = myStartRow;
		storedBinIend[fromWho+1] = myEndRow;
		cout << "Rank : " << myRank << " stored bin " << endl;
		for (int itest = 1; itest< maxRank+1; itest++){
	//		cout << storedBinIstart[itest] << " : " << storedBinIend[itest] << endl;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(myRank ==fromWho){
		myAmountToSend = 4*(myBinI[toWho+1]-myBinI[toWho]);
		mySendStartingPoint = 4*(myBinI[toWho]);
		MPI_Isend(&myArray[mySendStartingPoint], myAmountToSend, MPI_DOUBLE, toWho, 888,  MPI_COMM_WORLD, &request); 
		cout << "Rank: "<< myRank << " sent myArray to: " << toWho << " amount " << myAmountToSend << endl;
	}	
	MPI_Barrier(MPI_COMM_WORLD);
	if(myRank == toWho){
		myAmountToReceive = 4*(myEndRow-myStartRow);
		double *receiveThis;
		receiveThis = (double*) malloc((myAmountToReceive)*sizeof(double));
		cout << "My rank is XXXXXXX here : " << myRank << "to who value is : " << toWho << " amount " << myAmountToReceive <<  endl;
		MPI_Recv(receiveThis, myAmountToReceive, MPI_DOUBLE, fromWho, 888 , MPI_COMM_WORLD,&status);
		cout << "Rank: " << myRank << " received array from " << fromWho << endl;
	//	for (int ci = 0; ci <myAmountToReceive; ci++){
		//	cout << receiveThis[ci] << endl;
	//	} 
	
		//append to an array	
		double *tempArray;
		tempArray = (double*) malloc(((rowPTR[0]*4)+(myAmountToReceive))*sizeof(double));
		for(int fill = 0; fill< (rowPTR[0]*4);fill++){
			tempArray[fill] = myArray[fill];
			cout << "tempArray[" << fill << "] = " << tempArray[fill] << std::endl;
		}
		for(int fill2 = (rowPTR[0]*4); fill2 < (rowPTR[0]*4)+myAmountToReceive; fill2++){
			tempArray[fill2] = receiveThis[fill2-rowPTR[0]*4];
		}
		for(int ci = 0; ci< (rowPTR[0]*4+myAmountToReceive); ci ++ ){
			//cout << tempArray[ci]<< " "; 
		}
//		cout << endl;


		cout << "Rank " << myRank << " ";

		cout << "myArray at the beginning" << endl;
		for (int i = 0; i <*rowPTR; i++) {
			cout << "Row " << i << " :";
			for (int j = 0; j < *colPTR; j++)
				cout << myArray[i * 4 + j] << " : ";
			cout << std::endl;
		}
		
		cout << "tempArray at the beginning" << endl;
		for (int i = 0; i <(*rowPTR+myAmountToReceive/4); i++) {
			cout << "Row " << i << " :";
			for (int j = 0; j < *colPTR; j++)
				cout << tempArray[i * 4 + j] << " : ";
			cout << std::endl;
		}

		*pmyArray = tempArray;

		rowPTR[0]= rowPTR[0]+ myAmountToReceive/4;
		cout << " IN function test array " << *rowPTR << endl;
		myArray[4] = 9999.99;
		for (int tp = 0; tp < rowPTR[0]; tp ++){
			for(int tx = 0; tx< 4; tx++){
				cout << (*pmyArray)[4*tp+tx] << " " ;
			}
			cout << endl;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);


return;
}













