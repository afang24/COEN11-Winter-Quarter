#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define NODE struct node
#define LIST struct list
#include <pthread.h>
#include <unistd.h>

//node with only name and the party size
struct node
{
	char name[20];
	int partySize;
	NODE *next;
};

//linked list with head and tail 
//each node depending on size has a head and tail intially pointing to null
struct list
{
	NODE *head;
	NODE *tail;
};

//Global 
extern pthread_mutex_t lock;
extern LIST array[4];	//array of linked list

//user functions
extern void insert(char guest[], int sizeofParty);		  	//user inserts a name and party size	
extern void list();		  	 				//outputs people in the list
extern void removeParty(int tableReady);		 		//removes guest from the list
extern void showGuest();						//show guest information from given name
extern int check_duplicate(char *str); 				//checks to see if the inputted name is already in the list
extern void delete_all(NODE *p);				        //free all nodes and quits program
extern void save_file(char *fileName, int keys);			//save information into a file with a random file name
extern void read_file(char *fileName, int keys);			//read a file to see if theres information inside, if there is, reinserts info into list
extern void showListBackwards(NODE *p);				//recursive function that shows the list in reverse order
extern void showNameBackwards(NODE *p);				//recursive function that shows the names in the list 
extern void *autosave(void *arg);					//autosave function taht saves info into a binary file every 15 seconds
extern void read_autosave(char *fileName);				//read autosave reads information from the binary file needs to grab the lock in case autosaving into file
								//unlocking is when a function is done with the lock they can release it and whoevers next in waitlist can grab the lock
								//lock -> basically permission to access unlocking frees the lock allowing other functions ot have the lock^

