#include "link.h"

//saves into binary file every 15 seconds
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
		pthread_mutex_unlock(&lock);
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

//save information into text fie
void save_file(char *file_name, int keys)
{
	FILE *fp;
	NODE *p;
	int i,j;
	char buffer[100];	
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
			sprintf(buffer, "%s\t%d\n", p->name, p->partySize);
			for(j = 0; buffer[j]!='\n';j++)
			{
				buffer[j]=((buffer[j])^(keys));
			}	
			fprintf(fp, "%s\n", buffer);
			p = p->next;
		}
	}
	fclose(fp);	
	return;	
}

//accepts a file name and reads the file
//if there is something in the file re-inserts the information
void read_file(char *file_name, int keys)
{
	FILE *fp;
	char fileName[20];
	int filepartySize;
	char buffer[100];
	int i;
	
	fp = fopen(file_name, "r");
	
	while(fp == NULL)	//txt file is empty so you can proceed
	{
		return;
	}

	fseek(fp, 31, SEEK_SET); 	//skips the header because when reading a file you just want the information

	while(fgets(buffer, 100, fp) != NULL)
	{
		for(i=0;buffer[i]!='\n';i++)	//decrypting
		{
			buffer[i]=((buffer[i])^(keys));
		}
		sscanf(buffer, "%s\t%d\n", fileName, &filepartySize);
		insert(fileName, filepartySize);	
	}	
	fclose(fp);
	return;
}

