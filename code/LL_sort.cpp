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
#include <cstdlib>

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



#include "LL_sort.h"
	
using namespace std;

struct Node{
	double ll_location;
	double ll_x;
	double ll_y;
	double ll_z;
	struct Node *next;
};


void LL_sort(double myArray[], int rows, int cols, int ll_sortThisColumn){
	cout << "Inside LL_sort before"<< endl;
	struct Node *ll_head = new Node();
	struct Node *ll_prev = new Node();
	struct Node *ll_cur = new Node();
	struct Node *ll_temp = new Node();
	int ll_foundLocation = 0;
	double ll_sortValue, ll_newValue;
	//prime 
	struct Node *ll_first = new Node();
	ll_prev->next = NULL;
	ll_cur->next = NULL;
	ll_head->next = NULL;
	ll_temp = NULL;
	ll_first->ll_location = myArray[0];
	ll_first->ll_x = myArray[1];
	ll_first->ll_y = myArray[2];
	ll_first->ll_z = myArray[3];
	ll_first->next = NULL;
	ll_head->next = ll_first;
	ll_cur = ll_head;	
	
	for( int ll_i=1; ll_i< rows; ll_i++){
		Node* ll_new = new Node();
		ll_new->ll_location = myArray[cols*ll_i+0];
		ll_new->ll_x = myArray[cols*ll_i+1];
		ll_new->ll_y = myArray[cols*ll_i+2];
		ll_new->ll_z = myArray[cols*ll_i+3];
		ll_new->next = NULL;
		
		ll_cur = ll_head;
		ll_prev = ll_head;
		//cout << "NEW INSERTING cur: " << ll_head->next->ll_location << endl;
		while((ll_foundLocation !=1)){
			if(ll_cur->next !=NULL){
				if(ll_sortThisColumn ==0){
					ll_sortValue = ll_cur->next->ll_location;
					ll_newValue = ll_new->ll_location;
				}
			//search x
				else if(ll_sortThisColumn ==1){
					ll_sortValue = ll_cur->next->ll_x;
					ll_newValue = ll_new->ll_x;
				}
			//search y
				else if(ll_sortThisColumn ==2){
					ll_sortValue = ll_cur->next->ll_y;
					ll_newValue = ll_new->ll_y;
				}
			//search z
				else if(ll_sortThisColumn ==3){
					ll_sortValue = ll_cur->next->ll_z;
					ll_newValue = ll_new->ll_z;
				}
				//cout << "sort values: ponter loc: " << ll_sortValue << " : new value " << ll_newValue << endl;			
		}	
			//seach with selected values
			if(ll_cur->next == NULL){ //insert at end
				//cout << "insert at end" << endl;
				ll_prev->next = ll_new;
				ll_foundLocation = 1;
			}
			else if(ll_sortValue >ll_newValue){//insert in midle somewhere
				//insert
				ll_new->next = ll_cur->next;
				ll_prev->next = ll_new;
				//cout << "insert bottom " << endl;
				ll_foundLocation = 1;
			}
			else{
			//incriment pointers
				//cout << "incriment pointer top " << endl;
				ll_prev= ll_prev->next;
				ll_cur = ll_cur->next;
				//cout << "incirment pointer bottom " << endl;	
			}
		//cout << "ll_cur at bottom of loop: " << ll_cur->ll_location << endl;		
		}
		ll_foundLocation = 0;
	}
	ll_cur = ll_head;
	ll_prev = ll_head->next;
	int lll = 0;
	while (ll_cur->next !=NULL){
		myArray[cols*(lll)]=ll_prev->ll_location;
		myArray[cols*(lll)+1]=ll_prev->ll_x ;
		myArray[cols*(lll)+2]=ll_prev->ll_y;
		myArray[cols*(lll)+3]=ll_prev->ll_z;
		
		//cout << ll_prev->ll_location << "\t" << ll_prev->ll_x << "\t" << ll_prev->ll_y <<"\t" << ll_prev->ll_z << endl;
		ll_cur = ll_cur->next;
		ll_prev = ll_prev->next;
		lll++;
		
	}
	//cout << "LL_sort complete" << endl;
	
return;
}

