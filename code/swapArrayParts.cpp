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

#include "swapArrayParts.h"
#include "definitions.h"


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

	if (toWho == fromWho)
		return;

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
                        myBinI[mi+1] = binIPTR[mi];
                }
                MPI_Isend(myBinI, (maxRank+1), MPI_INT, toWho,999, MPI_COMM_WORLD, &request);
      //          cout << "Rank: " << myRank << " has sent ibin to: " << toWho << endl;
        }
        if(myRank ==toWho){
                MPI_Recv(yourBinI, (maxRank+1), MPI_INT, fromWho , 999, MPI_COMM_WORLD, &status);
                myStartRow = yourBinI[myRank];
                myEndRow = yourBinI[myrank+1];
                storedBinIstart[fromWho+1] = myStartRow;
                storedBinIend[fromWho+1] = myEndRow;
 //               sleep(myRank);
        //        cout << "Rank: " << myRank<< " has received binI from: " << fromWho << endl;
         //       for (int itest = 0; itest< maxRank+1; itest++){
          //              cout << yourBinI[itest] << " " ;
           //     }
            //    cout << endl;
        }

        if(myRank == fromWho){
                myAmountToSend = 4*(myBinI[toWho+1]-myBinI[toWho]);

		if (myAmountToSend < 0) {
			cout << "Error! Amount to send is negative!" << endl
				<< "Rank " << myRank << endl
				<< "toWho " << toWho << " fromWho " << fromWho << endl
				<< "myBinI[" << toWho+1 << "] = " << myBinI[toWho+1] << endl
				<< "myBinI[" << toWho << "] = " << myBinI[toWho] << endl;
			MPI_Abort(MPI_COMM_WORLD, _FAIL_);
		}

                mySendStartingPoint = 4*(myBinI[toWho]);
		cout << "Rank " << myRank << " is sending " << myAmountToSend << " to " << toWho << endl;
                MPI_Isend(&myArray[mySendStartingPoint], myAmountToSend, MPI_DOUBLE, toWho, 888,  MPI_COMM_WORLD, &request);
              //  cout << "Rank: " << myRank << " has sent the array to: " << toWho << " of size " << myAmountToSend << endl;
                //cout << "Last Rank: " << myRank << " received: loc-" << myArray[4*999] << " :x- " << myArray[4*999+1] << " :y- " << myArray[4*999+2] << " :z- " <<myArray[999*4+3]<< endl;
        }
        if(myRank == toWho){
                myAmountToReceive = 4*(myEndRow-myStartRow);
             //   cout << "Rank: " << myRank << " is attempting to get: " << myAmountToReceive << endl;
                double *receiveThis;
                receiveThis = (double*) malloc((myAmountToReceive)*sizeof(double));
                MPI_Recv(receiveThis, myAmountToReceive, MPI_DOUBLE, fromWho, 888 , MPI_COMM_WORLD,&status);
            //    cout << "Rank: " << myRank << " has received the array from " << fromWho << endl;
                double *tempArray;
                tempArray = (double*) malloc(((rowPTR[0]*4)+(myAmountToReceive))*sizeof(double));
           //     cout << "Rank: " << myRank << " after malloc in array receive " << fromWho << endl;
                for(int fill = 0; fill< (rowPTR[0]*4);fill++){
//                        cout << "FILL: " << fill <<  " rowPTR[0]*4: " << rowPTR[0]*4 << " lenth of temp: "<< ((rowPTR[0]*4)+(myAmountToReceive)) << " myamounttoreceive: " << myAmountToReceive << endl;
                        tempArray[fill] = myArray[fill];
                }
            //    cout << "Rank: " << myRank << " filled my array " << fromWho << endl;
                for(int fill2 = (rowPTR[0]*4); fill2 < (rowPTR[0]*4)+myAmountToReceive; fill2++){
                        tempArray[fill2] = receiveThis[fill2-rowPTR[0]*4];
                }
           //     cout << "rank: " << myRank << " filled my array with new data " << endl;
                free(*pmyArray);
                *pmyArray = tempArray;
             //   cout << "Row 1 Rank: " << myRank << " received: loc-" << tempArray[0] << " :x- " << tempArray[1] << " :y- " << tempArray[2] << " :z- " << tempArray[3]<< endl;

                rowPTR[0]= rowPTR[0]+ myAmountToReceive/4;
        }
      //  cout << "rank: " << myRank << " is at the bottom of swap of from " << fromWho<< " to: " << toWho  << endl;


return;
}













