#include<stdlib.h>
#include<stdio.h>
#include<time.h>


int divide();					//divide function called by main to perform calculations
int counter = 0;							//counter to show progress
int n;
int count = 0;								//final counter
int quotient;

int main(void)
{        								//loop
        srand((int)time(NULL));
        for(int i = 0; i < 10; i++){
                int quotient = rand () % 13;	 			//generate random quotient (top)
                int divisor = rand () % 12 + 1;  			//generate random divisor (bottom)

                n = divide(divisor, quotient);
                if(n == 1){    						//if the user input correct answer then the divide function returns 1 else it would return 0
                    count ++;  						//this is the final counter
                }

        }
        printf("Final Score: %d / 10\n", count);
}


int divide(int divisor, int quotient){
               
               int userQuotient;
               int dividend = quotient * divisor;
               
                printf("What is %d/%d?\n", dividend, divisor);

                scanf("%d",&userQuotient); 				 //ask user to input a quotient

                if(userQuotient == quotient)
                {
                    printf("CORRECT!\n");
                    counter ++;
                    printf("Count is currently: %d\n", counter);
                    printf("\n");
		    return 1;
            
                }
                else
                {
                	printf("INCORRECT!\n");
                	printf("Correct answer is %d\n",quotient);
                	printf("\n");
			return 0;
                }

}




