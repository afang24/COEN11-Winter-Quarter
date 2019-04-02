#include "link.h"

LIST array[4];   //array of linked list
pthread_mutex_t lock;


int main (int argc, char *argv[])	//argc = number of arguments	//argv = file name   	//argv[2] binary file	//argv[3] key to decrypt
{
	char name[20];
	int size;
	int tableReady;
	int old, new;	
	int i;
	NODE *p;
	int key;	
	
	for(i=0;i<4;i++){
		array[i].head=NULL;
		array[i].tail=NULL;
	}

	if(argc!=4)
	{
		printf("ERROR: NOT ENEOUGH ARGUMENTS");
		return 0;
	}
	key=atoi(argv[3]);
	read_file(argv[1],key);
	printf("compiling readfile...success\n");

	//creating thread and lock
	pthread_t thread;
	pthread_mutex_init (&lock, NULL);
	pthread_create (&thread, NULL, autosave, (void *)argv[2]);
	printf("compiling thread...success\n");

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

		int input;
		scanf("%d",&input);      //prompts user to input their choice and assigns their answer to input

		switch(input) 		
		{
			case 0:			//quits program					
				pthread_mutex_lock(&lock);
				printf("Gaining access to lock...\n");
				pthread_cancel(thread);
				printf("Canceling thread...\n");
				pthread_mutex_unlock(&lock);
				printf("Success at canceling thread.\n");

				   //end program
                                printf("Starting.\n");
                                save_file(argv[1],key);
                                printf("Successfully saved waitlist into textfile.\n");
				printf("Thank you for eating at Alex's Asian Cuisines come again!\n");
				printf("Freeing nodes...\n");
				for(i = 0; i < 4; i++)
				{	
					p = array[i].head;
					delete_all(p);				//freeing all the nodes
				}
				printf("Freeing nodes successful.\n");
				return 0;
			case 1:
				printf("What is your name?\n");
				scanf("%s",name);
				if(check_duplicate(name) == 1)
				{
					printf("sorry name is list already");
					break;
				}
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
			default:
				printf("invalid choice. Try again\n");
				break;
		}
	}
}

