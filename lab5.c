/*
	Alex Fang	
	COEN 11 || lab5.c
	2/6/19
	Description: lab derived from lab3.c, but instead modifying program to use 
	linked list instead of an array. Lab for learning about Nodes.
*/

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define NODE struct node


//node with only name and the party size
struct node
{
	char name[20];
	int partySize;
	NODE *next;
};


//global variables
NODE *head = NULL;
NODE *tail = NULL;


//user functions
void insert();		  	 //user inserts a name and party size	
void list();		  	 //outputs people in the list
void removeParty();		 //removes guest from the list
void showGuest();		 //show guest information from given name
int check_duplicate(char *str);  //checks to see if the inputted name is already in the list


int main (void)
{
	while (1) 		 //loop function that loops forever until user stops it  
	{
		printf("\n");
		printf("Welcome to our resturant! What does guest want to do:\n");
		printf("0)Exit this waiting list.\n");
		printf("1)Reserve a table.\n");
		printf("2)Remove your name off the waitinglist\n");
		printf("3)View Current waitlist\n");
		printf("4)Show entries less than or equal to entered size\n");

		int input;
		scanf("%d",&input);      //prompts user to input their choice and assigns their answer to input

		switch(input) 		
		{
			case 0:			//quits program
				printf("Thank you come again!\n");
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
	NODE *p;
	p = (NODE*)malloc(sizeof (NODE));  //malloc is used to create an unallocated node in memory to populate
	
	printf("What is the guest's name?\n");
	scanf("%s",names);
	if(check_duplicate(names) == 1)
	{ 					//checkduplicate returns 1 if there is a name that matches the guest's name
		printf("Sorry name is already on the list. Try again.\n");
		return;
	}
	printf("How many people are in your party?\n");
	scanf("%d",&size);
	if(size > 10)
	{
		printf("sorry party is too big.\n");
		return;
	}
	p->partySize = size;
	strcpy(p->name,names);
	printf("You have successfully been added to the list.\n");
	if(head == NULL)
	{
		p->next = NULL;
		tail = head = p;
		return;
	}
	else
	{				
		p->next = NULL;
		tail->next = p;
		tail = p;
		return;
	}	
	free(p);	

}


void list()
{
	NODE *p;
	p = head;

	printf("Name      | Party Size\n");
	while(p != NULL)
	{
		printf("%s\t\t%d\n",p->name, p->partySize);
		p = p->next;
	}
	return;
}

void removeParty()
{
	int size;
	NODE *p,*q;	   //using two pointers with q trailng p
	p = q = head;
	
	printf("There is a table ready for what size party?\n");
	scanf("%d",&size);

	while(p != NULL)
	{
		if(p->partySize  == size)
		{
			break;
		}
		q = p;
		p = p->next;	
	}

	if(p == NULL)  	//if the size is not found
	{
		printf("not found\n");
		return;
	}	

	if(p == head)	//answer is in first node
	{
		head = p->next;
		free(p);
		printf("Success\n");
		return;
	}
	else if(p == tail)	//answer is in the last node
	{
		tail = q;
		q->next = NULL;
		free(p);
		printf("Success\n");
		return;	
	}
	else	//answer is in one of the middle nodes
	{
		q->next = p->next;
		free(p);
		printf("Success\n");
		return;	
	}
	
}

void showGuest()
{
	int guestSize;
	NODE *p;
	p = head;	

	printf("What party size would you like to see?\n");
	scanf("%d",&guestSize);

	while(p != NULL)
	{
		if(p->partySize <= guestSize)
		{
			printf("%s\t\t%d\n",p->name, p->partySize);
		}
		p = p->next;
	}
	return;
}


int check_duplicate(char *str)
{
	NODE *p;
	p = head;
	
	while(p != NULL)
	{
		if(strcmp(str,p->name) == 0)   //strcmp returns 0 if true 
		{
			return 1;		
		}
		p = p->next;
	}
	return 0;
}



