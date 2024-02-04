// Fram Work

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FIELDS_LENGTH 50
#define MENU_LENGTH 200
#define FIELD_VALUE_LENGTH 20
#define FIELDS_FILE_NAME "StudentFields.cnf"
#define MENU_FILE_NAME "StudentMenu.cnf"
#define DATA_FILE_NAME "StudentsDetails.dat"
#define PRINT_CNF_FILE_NAME "StudentsPrintfStatements.cnf"

void showMenu();
void Load(char **, char *, int);
void LoadPrintStatements();
int Counter(char*);
void create();
void showAll();
long long int getRecordPosition( char * );
void search( char * );
void printNotFoundMsg( char * );
void Update( char *, int);
void printSucessMsg();
void printMsg(char *, int);
char* readId();

FILE *FpMenu; 
FILE *FpFields; 
FILE *FpData;
FILE *FpPrintStatements;

int FieldsCounter;
int PrintStatementsCounter;

char **ptrFieldNames;

char **ptrPrintStatements;

char *ptrFieldValue;




void main()
{ 
	

	ptrFieldNames = (char**) malloc(sizeof(char*) * FieldsCounter);

	ptrPrintStatements = (char**) malloc(sizeof(char*) * PrintStatementsCounter);
	
	Load(ptrFieldNames, FIELDS_FILE_NAME, FieldsCounter = Counter(FIELDS_FILE_NAME));
	printf("%s", ptrFieldNames[1]);

	Load(ptrPrintStatements, PRINT_CNF_FILE_NAME, PrintStatementsCounter = Counter(PRINT_CNF_FILE_NAME));

	printf( ptrPrintStatements[1], "Rakesh");





	while(1){
		showMenu();
	}

}

void showMenu()
{
	char Menu[MENU_LENGTH];
	FpMenu = fopen(MENU_FILE_NAME, "r");

	
	printf("\n");

	while (fgets(Menu, MENU_LENGTH, FpMenu))
	{
		printf("%s\n", Menu);
	}

	fclose(FpMenu);

	int Counter;
	int choice;
	
	char *Id;

	printf("Enter your choice: ");
	scanf("%d", &choice);

	switch(choice)
	{
		case 1: create();
				break;

		case 2: showAll();
				break; 

		case 3: printf("\nTo search.\n ");
				search(readId());
				break;

		case 4: printf("To Update.\n");
		case 5: Update(readId(), choice);
				break;

		case 6: exit(0);
	}
}

char* readId()
{
	printf("\nEnter Id: ");
	char *Id = (char*) malloc(sizeof(char) * FIELD_VALUE_LENGTH);
	scanf("%s",Id);
	return Id;
}

int Counter(char *FileName)
{
	char Statements[MENU_LENGTH];

	int Counter = 0;

	FILE *Fp = fopen(FileName, "r");

	while (fgets(Statements, MENU_LENGTH, Fp))
		Counter++;

	fclose(FpFields);
	return Counter;

}

void Load(char **ptr, char *FileName, int Count )
{
	FpFields = fopen(FileName, "r");

	

	int Counter = 0;



	while (Counter < Count)
	{
		ptr[Counter] = (char*) malloc(sizeof(char) * FIELDS_LENGTH);
		fgets(ptr[Counter], FIELDS_LENGTH, FpFields);

		ptr[Counter][strlen(ptr[Counter]) - 1] = '\0';
		Counter++;

	}

	fclose(FpFields);


}


void create()
{
	FpData = fopen(DATA_FILE_NAME, "a");

	fputc('A', FpData);

	int Counter = 0;

	
	ptrFieldValue = (char*) malloc(sizeof(char) * FIELD_VALUE_LENGTH);

	while (Counter < FieldsCounter)
	{
		printf("Enter %s: ", ptrFieldNames[Counter]);
		scanf("%s", ptrFieldValue);
		fwrite(ptrFieldValue, FIELD_VALUE_LENGTH, 1, FpData);
		Counter++;
	}

	fclose(FpData);
	printMsg(NULL, 1);
	free(ptrFieldValue);

}


void showAll()
{
	FpData = fopen(DATA_FILE_NAME, "r");

	ptrFieldValue = (char*) malloc(sizeof(char) * FIELD_VALUE_LENGTH);

	int Counter;

	int FileStatus = 0;

	int found = 0;

	while(  !feof(FpData) )
	{
		char Status = fgetc(FpData);
		if (Status == 'A')
		{
			Counter = 0;

			printf("");
			while( Counter < FieldsCounter )
			{
				fread(ptrFieldValue, sizeof(char) * FIELD_VALUE_LENGTH, 1, FpData);
				printf("%s: %s\n", ptrFieldNames[Counter], ptrFieldValue);
				Counter++;

			}

			found++;

		}

		else if( Status == 'D')
		{
			fseek( FpData, sizeof(char) * (FIELD_VALUE_LENGTH * FieldsCounter), SEEK_CUR );
		}
	}

	if (found == 0)
	{
		printMsg(NULL, 4);

	}


	free(ptrFieldValue);
	fclose(FpData);

}


long long int getRecordPosition(char *Id)
{
	FpData = fopen(DATA_FILE_NAME, "r");

	ptrFieldValue = (char*) malloc(sizeof(char) * FIELD_VALUE_LENGTH);

	long long int RecordPosition = -1;

	while( !feof(FpData) && RecordPosition == -1 )
	{
		char Status = fgetc(FpData);
		if ( Status == 'A')
		{
			fread(ptrFieldValue, sizeof(char) * FIELD_VALUE_LENGTH, 1, FpData);

			if(!strcmp(ptrFieldValue, Id))
			{
				RecordPosition = ftell(FpData);
			}
		}

	}

	free(ptrFieldValue);
	fclose(FpData);
	return RecordPosition;
}

void search(char *Id)
{
	long long int RecordPosition = getRecordPosition(Id);


	if(RecordPosition > -1)
	{
		FpData = fopen(DATA_FILE_NAME, "r");

		ptrFieldValue = (char*) malloc(sizeof(char) * FIELD_VALUE_LENGTH);
		fseek(FpData, RecordPosition, SEEK_SET);

		int Counter = 1;

		while (Counter < FieldsCounter)
		{
			fread(ptrFieldValue, sizeof(char) * FIELD_VALUE_LENGTH, 1, FpData);
			printf("%s : %s\n", ptrFieldNames[Counter], ptrFieldValue);
			Counter++;

		}
	}

	else
	{
		printMsg(Id, 0);
	}
}


void Update(char *Id, int choice)
{
	long long int RecordPosition = getRecordPosition(Id);


	if(RecordPosition > -1)
	{
		
		ptrFieldValue = (char*) malloc(sizeof(char) * FIELD_VALUE_LENGTH);
		

		if( choice == 4)
		{
			int Counter = 1;

			while (Counter < FieldsCounter)
			{
				printf("Enter new %s: ", ptrFieldNames[Counter]);
				scanf("%s", ptrFieldValue);

				FpData = fopen(DATA_FILE_NAME, "r+");

				fseek(FpData, RecordPosition, SEEK_SET);
				fwrite(ptrFieldValue, sizeof(char) * FIELD_VALUE_LENGTH, 1, FpData);

				fclose(FpData);

				Counter++;
				RecordPosition += FIELD_VALUE_LENGTH;

			}
			printMsg(Id, 3);
		}

		else
		{
			FpData = fopen(DATA_FILE_NAME, "r+");
			fseek(FpData, RecordPosition - (FIELD_VALUE_LENGTH + 1), SEEK_SET);
			fputc('D', FpData);
			printMsg(Id, 2);
			fclose(FpData);
		}

	}

	else
	{
		printMsg(Id, 0);
	}

}


void printMsg(char *Id, int Index)
{
	if (Id == NULL)
		printf(ptrPrintStatements[Index]);
	else
		printf(ptrPrintStatements[Index], Id);
}