/*
	Alex Fang	
	COEN 11 || lab6.c
	2/13/19
	Description: Using linked list with a head and tail, and an array of struct list
	waiting list now consist of 4 age range groups
*/

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define NODE struct node
#define LIST struct list

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

LIST array[4] ={{NULL,NULL}, {NULL,NULL}, {NULL,NULL}, {NULL, NULL}};	//array of linked list

//user functions
void insert();		  	 //user inserts a name and party size	
void list();		  	 //outputs people in the list
void removeParty();		 //removes guest from the list
void showGuest();		 //show guest information from given name
int check_duplicate(char *str);  //checks to see if the inputted name is already in the list
void delete_all();		 //free all nodes and quits program
void changeSize(char str[], int old, int new); 	 //change the size of a group for a reservation



int main (void)
{
	char name[20];
	int old, new;	

	while (1) 		 //loop function that loops forever until user stops it  
	{
		printf("\n");
		printf("Welcome to our resturant! What does guest want to do:\n");
		printf("[0]Exit this waiting list.\n");
		printf("[1]Reserve a table.\n");
		printf("[2]Remove your name off the waitinglist\n");
		printf("[3]View Current waitlist\n");
		printf("[4]Show entries less than or equal to entered size\n");
		printf("[5]Change the size of a group for a reservation\n");  

		int input;
		scanf("%d",&input);      //prompts user to input their choice and assigns their answer to input

		switch(input) 		
		{
			case 0:			//quits program
				printf("Thank you come again!\n");
				delete_all();
				return 0;
			case 1:		
				insert();
				break;
			case 2:
				removeParty();		
				break;
			case 3:	
				list();
				break;
			case 4:
				showGuest();
				break;
			case 5:
				scanf("What is party's name:%s\n",name);
				scanf("What is party's old size:%d\n",&old);
				scanf("What is party's new size:%d\n",&new);
				//changeSize(name, old, new);
				break;
			default:
				printf("invalid choice. Try again\n");
				break;
		}
	}
}

void insert()
{
	char names[20];
	int size;
	NODE *p;				//temporary node
	p = (NODE*)malloc(sizeof(NODE));	//malloc is used to create an unallocated node in memory to populate
	int i;					//which nodes is the one we are looking at

	
	printf("What is the guest's name?\n");
	scanf("%s",names);
	if(check_duplicate(names) == 1)
	{ 					//checkduplicate returns 1 if there is a name that matches the guest's name
		printf("Sorry name is already on the list. Try again.\n");
		return;
	}
	printf("How many people are in your party?\n");
	scanf("%d",&size);
	p->partySize = size;
	strcpy(p->name, names);
	
	switch(size)
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
	printf("You have successfully been added to the list.\n");		

}


void list()
{
	NODE *p;
	int i;
	
	printf("Name      | Party Size\n");
	for(i = 0; i < 4; i++)
	{
		p = array[i].head;
		while(p != NULL)
		{
			printf("%s\t\t%d\n",p->name, p->partySize);
			p = p->next;
		}
	}
	return;
}

void removeParty()
{
	int size;
	NODE *p,*q;	   //using two pointers with q trailng p
	int i;
	int j;
	
	printf("There is a table ready for what size party?\n");
	scanf("%d",&size);

	//this is just to see which node p is pointing to in the array (age range)
	if(size == 1 || size == 2)
	{
		i = 0;
	
	}
	else if(size == 3 || size == 4)
	{
		i = 1;
	
	}
	else if(size == 5|| size == 6)
	{
		i = 2;
			
	}
	else if(size >= 7) 		//size greater than 7
	{
		i = 3;
		
	}
	
	//this is if the condition is true then you can look through the list
	//if not then loop through and this is for if the answer is in the middle of the node
	
	for(j = 0; j <= i; j++)
	{
		p = array[j].head;
		while(p != NULL)
		{
			if(p->partySize <= size)
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

void delete_all(void)
{
   	NODE *p,*q;
  	int i;
  	for(i = 0; i < 4; i++)
  	{
    		p = array[i].head;
   		while(p != NULL)
    		{
      			q = p;
      			p = p->next;
      			free(q);
    		}
  	}	
  	printf("List Deleted\n");
  	return;
}

/*
void changeSize(char str[], int old, int new )
{
	NODE *p,*q;
	int i;
	int tempSize;

	while(array[old].head  != NULL)
	{
		if(strcmp(str, p->name) == 0)
		{			
			             //deletes it
    
							
			p->partySize = new;					
			p->next = NULL;					//add new size to respective size in array 
			array[new].tail->next = p;
                        array[new].tail = p;
	                return;												
		}	
		else
		{
			printf("Name not found in this list. Try a different name.\n");
		}
	}	

}
*/



