
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"
#include <string.h>


//   mpiexec -n 3 --prepend-rank ./p5 CMDS


int main(int argc,char *argv[])
{
    alarm(300);
    int rc, myrank, numranks, mytag, n, i;
    unsigned myflag =0;
    rc = MPI_Init(&argc,&argv);  // NULL,NULL ; rc == MPI_SUCCESS
    rc = MPI_Comm_size(MPI_COMM_WORLD,&numranks);
    rc = MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&numranks);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Status pstatus, status;
    
    char *first_input_name_of_program = argv[0];
	char  *FileName = (argv[1]);
	//printf("NUM RANKS: %d\n", numranks);
if(numranks>1){
if(myrank == 0){
	int buffer;
	unsigned buffer2, read;
	FILE *readin;
	char *readtohere = NULL;
	size_t sizeofread = 0;
	readin = fopen(FileName, "r");
	while ((read = getline(&readtohere, &sizeofread, readin)) != -1) {
		//add send/receive her	
		MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		buffer2 = read;
		printf("Zero has sent a message to %d\n", buffer);
		rc = MPI_Ssend(&buffer2,1,MPI_UNSIGNED,buffer,999,MPI_COMM_WORLD);
		rc = MPI_Ssend(readtohere, buffer2,MPI_INT,buffer,999,MPI_COMM_WORLD);
	}
	free(readtohere);
	fclose(readin);
	//close everything down
	for(i=1; i<numranks;i++){
		MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		buffer2 = 1;
		rc = MPI_Ssend(&buffer2,1,MPI_UNSIGNED,buffer,999,MPI_COMM_WORLD);
	}
		
}

if(myrank!=0){
	while(myflag ==0){
		char mymessage[2000];
		int mylength;
		int sendbuf = myrank;
		unsigned newlength;
		rc = MPI_Ssend(&sendbuf,1,MPI_INT,0,999,MPI_COMM_WORLD);
		MPI_Recv(&newlength,1,MPI_UNSIGNED,0,999,MPI_COMM_WORLD, &status);
		//printf("rank tester %d\n", myrank);
		if(newlength != 1){	
			MPI_Recv(&mymessage,newlength, MPI_INT,0,999,MPI_COMM_WORLD, &status);
			//printf("My Rank %d\t Message: %s", myrank, &mymessage);
			//acutaly run the stuff
			system(mymessage);
		}
		else{
			myflag = 1;
		}
		
	}
}
}
if(numranks ==1){
	printf("You must enter more than 1 rank\n");
}
MPI_Barrier(MPI_COMM_WORLD);

exit(EXIT_SUCCESS);	
MPI_Finalize();
}
