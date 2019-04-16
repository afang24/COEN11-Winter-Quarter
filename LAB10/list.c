#include "link.h"

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
	{
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

