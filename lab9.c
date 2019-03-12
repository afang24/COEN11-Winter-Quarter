/*
	Alex Fang	
	COEN 11 || lab9.c
	2/27/19
	Description: derived from lab9.c, using threads to auto-save information into a binary file
	then new function called read_autosave to read from the binary file
*/

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define NODE struct node
#define LIST struct list
#include<pthread.h>
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
pthread_mutex_t lock;
LIST array[4] ={{NULL,NULL}, {NULL,NULL}, {NULL,NULL}, {NULL, NULL}};	//array of linked list

//user functions
void insert(char guest[], int sizeofParty);		  	//user inserts a name and party size	
void list();		  	 				//outputs people in the list
void removeParty(int tableReady);		 		//removes guest from the list
void showGuest();						//show guest information from given name
int check_duplicate(char *str); 				//checks to see if the inputted name is already in the list
void delete_all(NODE *p);				        //free all nodes and quits program
void changeSize(char str[], int old, int new); 			//change the size of a group for a reservation
void save_file(char *fileName);					//save information into a file with a random file name
void read_file(char *fileName);					//read a file to see if theres information inside, if there is, reinserts info into list
void showListBackwards(NODE *p);				//recursive function that shows the list in reverse order
void showNameBackwards(NODE *p);				//recursive function that shows the names in the list 
void *autosave(void *arg);					//autosave function taht saves info into a binary file every 15 seconds
void read_autosave(char *fileName);				//read autosave reads information from the binary file needs to grab the lock in case autosaving into file
								//unlocking is when a function is done with the lock they can release it and whoevers next in waitlist can grab the lock
								//lock -> basically permission to access unlocking frees the lock allowing other functions ot have the lock^


int main (int argc, char *argv[])	//argc = number of arguments	//argv = file name   	//argv[2] binary file
{
	char name[20];
	int size;
	int tableReady;
	int old, new;	
	int i;
	NODE *p;
	
	if(argc == 1)
	{
		printf("No wait list data availible. Trying to load default file.\n");
		read_file("init");
	}
	else
	{
		read_file(argv[1]);
	}	
	//create thread
	pthread_t thread;
	if(argc == 3)
	{
		pthread_create(&thread,NULL,autosave,(void *) argv[2]);
	}
	else
	{
		pthread_create(&thread,NULL,autosave,(void *) "temp.bin");
	}
	pthread_mutex_init(&lock, NULL);


	while (1) 		 //loop function that loops forever until user stops it  
	{
		printf("\n");
		printf("Welcome to Alex's Asian Cuisines resturant! What does guest want to do:\n");
		printf("[0]Exit this waiting list.\n");
		printf("[1]Reserve a table.\n");
		printf("[2]Remove your name off the waitinglist\n");
		printf("[3]View Current waitlist\n");
		printf("[4]Show entries less than or equal to entered size\n");
		printf("[5]Show list reversed\n");
		printf("[6]Show names in list reversed\n");
		printf("[7]Read from the binary file\n");
		//printf("[5]Change the size of a group for a reservation\n");  

		int input;
		scanf("%d",&input);      //prompts user to input their choice and assigns their answer to input

		switch(input) 		
		{
			case 0:			//quits program
				if(argc == 1)
				{
					save_file("init");
					printf("Successfully saved waitlist.\n");
				}
				//Update file
				else
				{
					save_file(argv[1]);
					printf("Successfully updated waitlist.\n");
				}
					
				pthread_mutex_lock(&lock);
				printf("Gaining access to lock...\n");
				pthread_cancel(thread);
				printf("Canceling thread...\n");
				pthread_mutex_unlock(&lock);
				printf("Success at canceling thread.\n");
				save_file(argv[1]);
				printf("Success at inserting into text file. \n");		
	
				printf("Thank you for eating at Alex's Asian Cuisines come again!\n");
				for(i = 0; i < 4; i++)
				{	
					p = array[i].head;
					delete_all(p);				//freeing all the nodes
				}
				return 0;
			case 1:
				printf("What is your name?\n");
				scanf("%s",name);
				printf("How many people are in your party?\n");
				scanf("%d",&size);
						
				insert(name, size);
				printf("You've been added to the list!\n");
				break;
			case 2:
				printf("There is a table ready for what party size:\n");
				scanf("%d", &tableReady);
				pthread_mutex_lock(&lock);
				removeParty(tableReady);
				pthread_mutex_unlock(&lock);		
				break;
			case 3:	
				list();
				break;
			case 4:
				showGuest();
				break;
			case 5:
				printf("Name\tPartySize\n");
				printf("--------------------\n");
				for(i = 0; i < 4; i++) {
					p = array[i].head;	
					showListBackwards(p);
				}	
				break;
			case 6:
				printf("Name\n");
				printf("-------\n");
				for(i = 0; i < 4; i++)
				{
					p = array[i].head;
					showNameBackwards(p);
				}
				break;
			case 7:
				printf("Reading from binary file...\n");
				read_autosave(argv[2]);
				break;
			/*case 5:
				printf("What is party's name:\n");
				scanf("%s",name);
				printf("What is party's old size:\n");
                                scanf("%d",&old);
				printf("What is party's new size:\n");
                                scanf("%d",&new);
				changeSize(name, old, new);
				break;
			*/
			default:
				printf("invalid choice. Try again\n");
				break;
		}
	}
}

void insert(char guest[], int sizeofParty)
{
	
	NODE *p;				//temporary node
	p = (NODE*)malloc(sizeof(NODE));	//malloc is used to create an unallocated node in memory to populate
	int i;					//which nodes is the one we are looking at

	if(p == (NODE *) NULL)
	{
		printf("Error allocaitng memory");
		return;
	}	

	if(check_duplicate(guest) == 1)
	{ 					//checkduplicate returns 1 if there is a name that matches the guest's name
		printf("Sorry name is already on the list. Try again.\n");
		return;
	}
	//gaining lock and changing list
	pthread_mutex_lock(&lock);  
	p->partySize = sizeofParty;
	strcpy(p->name, guest);
 
	switch(sizeofParty)
	{
		case 1: 			
		case 2:
			i = 0;
			break;
		case 3:
		case 4:
			i = 1;
			break;
 		case 5:
		case 6:
			i = 2;
			break;
		default:
			i = 3; 
			break;
	}
	
	//link the structure
	if(array[i].head == NULL)
	{
		array[i].head = array[i].tail = p;
		p->next = NULL;	
	}
	else
	{
		array[i].tail->next = p;
		array[i].tail = p;
		p->next = NULL;	
	}
	pthread_mutex_unlock(&lock);
}


void list()
{
	NODE *p;
	int i;

	printf("Name\tPartySize\n");
        printf("-----------------\n");	
	for(i = 0; i < 4; i++)
	{
		p = array[i].head;
		while(p != NULL)
		{
			printf("%s\t%d\n",p->name, p->partySize);
			p = p->next;
		}
	}
	return;
}

void removeParty(int tableReady)
{
	NODE *p,*q;	   //using two pointers with q trailng p
	int i;
	int j;
	

	//this is just to see which node p is pointing to in the array (age range)
	if(tableReady == 1 || tableReady == 2)
	{
		i = 0;
	
	}
	else if(tableReady == 3 || tableReady == 4)
	{
		i = 1;
	
	}
	else if(tableReady == 5|| tableReady == 6)
	{
		i = 2;
			
	}
	else if(tableReady >= 7) 		//size greater than 7
	{
		i = 3;
		
	}
	
	//this is if the condition is true then you can look through the list
	//if not then loop through and this is for if the answer is in the middle of the node
	
	//pthread_mutex_lock(&lock);	
	for(j = 0; j <= i; j++)
	{
		p = array[j].head;
		while(p != NULL)
		{
			if(p->partySize <= tableReady)
			{	
				break;
			}
			q = p;
			p = p->next;	
		}
			
	 	if(p == NULL)   //if the size is not found
         	{
                	 printf("not found\n");
      			 continue;
         	}
 
 	        if(p ==  array[j].head)   //answer is in first node
        	{
			p = array[j].head;
               	 	array[j].head = p->next; 
               	 	free(p);
                	printf("Success\n");
                	return;
         	}
         	else if(p == array[j].tail)      //answer is in the last node
         	{
                	 array[j].tail = q;
                 	q->next = NULL;
                 	free(p);
                 	printf("Success\n");
                 	return;
         	}
         	else    //answer is in one of the middle nodes
         	{
                 	q->next = p->next;
                 	free(p);
                	 printf("Success\n");
                 	return;
         	}
	}
	//pthread_mutex_unlock(&lock);
	printf("Cannot delete.\n");
	return;
}

void showGuest()
{
	int guestSize;
	NODE *p;
	p = (NODE*)malloc(sizeof (NODE));
	int i;

	printf("What party size would you like to see?\n");
	scanf("%d",&guestSize);

	printf("Name\tPartySize\n");
        printf("-----------------\n");
	for(i = 0; i < 4; i++)
	{
		p = array[i].head;
		while(p != NULL)
		{
			if(p->partySize <= guestSize)
			{
				printf("%s\t\t%d\n",p->name, p->partySize);
			}
			p = p->next;
		}
	}
	return;
}

//checks to see existing name is in the list
int check_duplicate(char *str)
{
	NODE *p;
	int i;
	
	for(i = 0; i < 4; i++)
	{	
		p = array[i].head; 
		while(p != NULL)
		{
				if(strcmp(str,p->name) == 0)   //strcmp returns 0 if true 
			{
				return 1;	//name exist	
			}
			p = p->next;
		}  	
	}
	return 0; 	//name doesn't exist
}

//recursion function to free all nodes in list
void delete_all(NODE *p)
{
	if(p == NULL)
	{
		return;
	}
	
	delete_all(p->next);	
	free(p);
  	return;
}

//save information into text fie
void save_file(char *file_name)
{
	FILE *fp;
	NODE *p;
	int i;
	
	fp = fopen(file_name, "w");

	while(fp == NULL)
	{
		printf("cannot open file");
		return;
	}
	
	fprintf(fp, "Name\tPartySize\n");
	fprintf(fp, "---------------\n"); 

	for(i = 0; i < 4; i++)
	{
		p = array[i].head;
		while(p != NULL)
		{
			fprintf(fp, "%s\t%d\n", p->name, p->partySize);
			p = p->next;
		}
	}
	fclose(fp);		
}

//accepts a file name and reads the file
//if there is something in the file re-inserts the information
void read_file(char *file_name)
{
	FILE *fp;
	char fileName[20];
	int filepartySize;

	fp = fopen(file_name, "r");
	
	while(fp == NULL)	//txt file is empty so you can proceed
	{
		return;
	}

	fseek(fp, 31, SEEK_SET);

	while(fscanf(fp, "%s %d\n", fileName, &filepartySize) == 2)
	{
		insert(fileName, filepartySize);		
	} 	
	return;
}

//reverse the entire list
void showListBackwards(NODE *p)		//break up list into two parts: the head and the rest of the list
{					//call this function for the rest of the linked list 
	if(p == NULL)
	{		//list is empty
		return;
	}

	showListBackwards(p->next);

	printf("%s\t%d\n", p->name,p->partySize);
	return;
}

//reverse each name in the list 

void reverseWord(char *array);
void reverseWord(char *array)
{
	if(*array == '\0')
	{
		return;
	}	
	
	reverseWord(array+1);
	printf("%c", *array);
}
void showNameBackwards(NODE *p)
{
         if(p == NULL)
         {
		return;
	 }
         reverseWord(p->name);
	 printf("\n");
	showNameBackwards(p->next);
         return;
}



//change size function not working yet
void changeSize(char str[], int old, int new )
{
	NODE *p,*q;
	int i;
	int tempSize;

	while(array[old].head  != NULL)
	{
		if(strcmp(str, p->name) == 0)
		{			
			removeParty(old);					             //deletes it					
			insert(p->name,new);					//add new size to respective size in array 
	                return;												
		}	
		else
		{
			printf("Name not found in this list. Try a different name.\n");
			return;
		}
	}	
	return;
}

//auto saves into a binary file every 15 seconds
void *autosave(void *arg)
{
	FILE *infp;
	int i;
	NODE *p;
	
	while(1)
	{
		sleep(15);
		if((infp = fopen((char *) arg, "wb")) == NULL)
		{
			printf("Error loading autosave file.\n");
		}
		else
		{
			pthread_mutex_lock(&lock);
			printf("\n");
			printf("Autosaving in progress...\n");
			for(i = 0; i < 4; i++)
			{
				p=array[i].head;
				while(p != NULL)
				{
					fwrite(p, sizeof(NODE), 1,infp);
					p = p->next; 	
				}
			}
			printf("success at writing into bin file\n");
			printf("\n");
			pthread_mutex_unlock(&lock);
		}
		fclose(infp);	
	}
}


//reads from the binary file
void read_autosave(char *fileName)
{
	FILE *infp;
	NODE p;
	pthread_mutex_lock(&lock);
	infp = fopen(fileName, "rb");
	
	//printf("1\n");
	if(infp == NULL)
	{
	//	printf("2\n");
		printf("Error opening binary fle.\n");
		return;
	}
	pthread_mutex_unlock(&lock);
	//printf("3\n");
	printf("Name\tParty Size\n");
	printf("-----------------\n");
	while(fread(&p, sizeof(NODE), 1, infp) == 1)
	{
	//	printf("4\n");
		printf("%s\t%d\n", p.name, p.partySize);	
	}
	fclose(infp);
	//printf("5\n");	
	return;
}



