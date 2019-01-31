#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>

typedef union reservation
{
	char specialName[20];
	int yearsTogether; 		
	int averageAge;
}EXTRA;


typedef struct waitlistperson
{
	int occasionType;
	char name[20];
	int partySize;
	EXTRA info;

}INFO;

int counter = 0;	//current size
INFO position[10];	//how big the list is

void insert();		  	 //declaring insert function	
void list();		  	 //declaring list function
void removeParty();		 //declaring remove function
void showGuest();
int check_duplicate(char *str);


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
	int i;
	char names[20];		//array that recieves guest's name
	int size;		//integer variable that is assinged user's input
	INFO *personp = &position[counter];	//pointing to the one after
	int special;
	int time;
	int average;
	int tempTime;
	char bdayName[20];
	int flag = 0; //if not special occasion

	if(counter < 10)
	{
		printf("What is the guest's name?\n");
		scanf("%s",names);
		i = check_duplicate(names);
		if(i == 0){
			return;
		}

		printf("How many people are in your party?\n");
		scanf("%d",&size);
		if(size > 10)
		{
			printf("sorry party is too big.\n");
			return;
		}
		
		printf("Type [1] if its someone's Birthday and type [2] if its an Anniversary, type [3] if neither.\n");
		scanf("%d",&special);		

		while(special < 1 || special > 3)
		{
			printf("invalid choice\n");
			scanf("%d",&special);	
		}

		switch (special)
		{
			case 1:
				printf("What is the name of the birthday person?\n");
				scanf("%s",bdayName);
				strcpy(personp->info.specialName,bdayName);
				personp->occasionType = special; 
				break;
			case 2:
				printf("How long were guest together?\n");
				scanf("%d",&time); 
				personp->info.yearsTogether = time;
				personp->occasionType = special;
				break;
			case 3:
				printf("What is the average age of group?\n");
				scanf("%d",&average);
				personp->occasionType = special;
				personp->info.averageAge = average;  
				break;
		}
	

		//personp += counter;
		strcpy(personp->name,names);
		personp->partySize = size; 		
		printf("Added!\n");
		personp++;
		counter++;
	}
	else
	{
		printf("Sorry all tables are taken. Come back when its free.\n");
		return;
	}	
}

void list()
{
	int k;
	INFO *personp = position;
	char birthday[] = "Birthday";
	char anniversary[] = "Anniversary";
	char otherwise[] = "Casual";
	
	printf("There are %d names on the waiting list:\n",counter);
	printf("\n");	

	printf("Name\t|\tParty Size\t|\tSpecial Occasion\t|\tExtra Info\n");
	for(k = 0; k < counter; k++, personp++)
	{	
		switch(personp->occasionType)
		{
			case 1:
				printf("%s\t\t\t%d\t\t\t%s\t\t\tName:%s\n",personp->name, personp->partySize, birthday, personp->info.specialName);
				break;
			case 2:
				printf("%s\t\t\t%d\t\t\t%s\t\t\t%dyears\n",personp->name, personp->partySize, anniversary, personp->info.yearsTogether);
				break;
			case 3:
				printf("%s\t\t\t%d\t\t\t%s\t\t\tAge:%d\n",personp->name, personp->partySize, otherwise,  personp->info.averageAge);
				break;
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
	INFO *personp = position;
	INFO *personq = position;

	
	printf("There is a table ready for what size party?\n");
	scanf("%d",&size);
		
	for(i = 0; i < counter; i++, personp++)
	{
		if(personp->partySize == size)
		{
			printf("%s with party size %d can be seated.\n", personp->name, personp->partySize);
			
			for(j = i; j < counter - 1; j++)         //starting from the next person on the waitiinglist
			{
				personq[j] = personq[j+1];		//copy next person into the previous person's spot because the person was seated	
			}
			i--;
			personp--;
			counter--;		//one less person on waitinglist
			flag++;
		}
	}
	
	
	if(flag == 0)
	{
		personp = position;	
		for(i = 0; i < counter; i++, personp++)
		{
			if(size >= personp->partySize)
			{
                         	printf("%s with party size %d can be seated.\n", personp->name, personp->partySize);
                                 
                         	for(j = i; j < counter - 1; j++)         //starting from the next person on the waitiinglist
                         	{       
                                	personq[j] = personq[j+1];              //copy next person into the previous person's spot because the person was seated        
                        	}       
                         	i--;    
                         	personp--;
                         	counter--;              //one less person on waitinglist
                         	flag1++;                
			}
		}
	
		if(flag1 == 0)
		{
			printf("Sorry no table can fit that size yet.\n");
		}
	}
	return;
}

void showGuest()
{
	INFO *personp = position;
	int i;
	int guestSize;
	char birthday[] = "Birthday";
        char anniversary[] = "Anniversary";
        char otherwise[] = "Casual";

	printf("What party size would you like to see?\n");
	scanf("%d",&guestSize);
	
	printf("Parties less than or equal to that size is \n");
	for(i = 0; i < counter; i++, personp++)
	{
		if(personp->partySize <= guestSize)
		{
			 switch(personp->occasionType)
                    {
                         case 1:
                                 printf("%s\t\t%d\t\t%s\t\tName:%s\n",personp->name, personp->partySize, birthday, personp->info.specialName);
                                 break;
                         case 2:
                                 printf("%s\t\t%d\t\t%s\t\t%dyears\n",personp->name, personp->partySize, anniversary, personp->info.yearsTogether);
                                 break;
                         case 3:
                                 printf("%s\t\t%d\t\t%s\t\tAge:%d\n",personp->name, personp->partySize, otherwise,  personp->info.averageAge);
                                 break;
                    }
          
		}
	}
	return;
}


int check_duplicate(char *str)
{
	INFO *personp = position;
	int i = 0;
	while(i < counter)
	{
		if(strcmp(str,personp->name) != 0)   //returns 0 if true 
		{
			i++;
			personp++;
		}
		else
		{
			printf("Sorry name is already taken!\n");
			return 0;
		}
	}
	return 1;

}
