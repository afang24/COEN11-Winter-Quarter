#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>


char nameList[10][20];   	 //declaring 10 row by 20 column matric for the user's name
int partySize[10];		 //declaring initial column matrix
int counter = 0;	 	 //declaring counter variable and number of people on the list

void insert();		  	 //declaring insert function	
void list();		  	 //declaring list function
void removeParty();		 //declaring remove function


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
			default:
				printf("invalid choice. Try again\n");
				break;
		}
	}
}

void insert()
{
	int i;
	char name[20];		//array that recieves guest's name
	int size;		//integer variable that is assinged user's input
	

		printf("What is the guest's name?\n");
		scanf("%s",name);
	

		printf("How many people are in your party?\n");
		scanf("%d",&size);
		if(size > 10)
		{
			printf("sorry party is too big.\n");
			return;
		}

		strcpy(nameList[counter],name);
		partySize[counter] = size; 		
		printf("Added!\n");
		counter++;

		if(counter == 10){
			printf("Sorry all tables are taken. Come back when its free.\n");
		}
		return;
			
}

void list()
{
	int k;

	printf("There are %d names on the waiting list:\n",counter);
	printf("\n");	

	printf("Name      | Party Size\n");
	for(k = 0; k < counter; k++)
	{
		if(nameList[k][0] != '\0')
		{
			printf("%s\t\t%d\n",nameList[k], partySize[k]);
		}
	}
	return;
}

void removeParty()
{
	int size;
	int i;
	int j;
	int flag = 0; 	   //tells you if there has been a value found
	int flag1 = 0;     //tells you if there is no table big enough for people on waitinglist

	if(counter != 0)
	{
		printf("There is a table ready for what size party?\n");
		scanf("%d",&size);
		
		for(i = 0; i < counter; i++)
		{
			if(size == partySize[i])
			{
				flag++;
				printf("%s with party size %d can be seated.\n", nameList[i], partySize[i]);
				for(j = i + 1; j < 10; j++)         //starting from the next person on the waitiinglist
				{
					strcpy(nameList[i],nameList[j]);		//copy next person into the previous person's spot because the person was seated
	                                partySize[i]=partySize[j];
					i++;
				}
				counter--;		//one less person on waitinglist
				break;
			}
		}
	}
	
	if(flag == 0)
	{
		for(i = 0; i < counter; i ++)
		{
			if(size >= partySize[i])
			{
				flag1 = 1;
				printf("%s with party size %d can be seated.\n", nameList[i], partySize[i]);
				for(j = i + 1; j < 10; j++)
				{
					strcpy(nameList[i],nameList[j]);
                                        partySize[i]=partySize[j];
                                        i++;
				}
				counter --;
				break;
			}
		}
		if(flag1 == 0)
		{
			printf("Sorry no table can fit that size yet.\n");
		}
	}
}


	





