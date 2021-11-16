#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _Stog;
typedef struct _Stog *Position;
typedef struct _Stog{
    double number;
    Position next;
} Stog;

int ReadFileAndCalculate(char* fileName, double* result);
int StringToPostfix(char* buffer, double* result);
Position CreateElement(double number);
int InsertAfter(Position head, Position new);
int DeleteAfter(Position prev);
int Push(Position head, double number);
int Pop(Position head, double* result);
int Calculate(Position head, char operation);

int main(){

    double *result = 0;

    ReadFileAndCalculate("postfix.txt", result);
    printf("Result: %.2f", &result);

    return 0;
}

int ReadFileAndCalculate(char* fileName, double* result){

    FILE *f = NULL;
    char *buffer = NULL;                // jer citamo sve, ne liniju po liniju
    int fileLength = 0;

    f = fopen(fileName, "rb");          // rb - read binary
    if(!f){
        perror("File cannot be opened! Please check your file and try again!\n");
        return -1;
    }

    fseek(f, 0, SEEK_END);              // idemo na kraj datoteke
    fileLength = ftell(f);              // broj byteova od pocetka do trenutne pozicije

    buffer = (char*)calloc((fileLength+1), sizeof(char));       // calloc - postavlja sve na 0, za razliku od malloc-a

    rewind(f);
    fread(buffer, sizeof(char), fileLength, f);                 // citamo CIJELU datoteku char po char, i spremamo je u buffer

    StringToPostfix(buffer, result);

    fclose(f);

    return EXIT_SUCCESS;
}

int StringToPostfix(char* buffer, double* result){

    Stog head = {.number = 0, .next = NULL};
    char* tempBuffer = NULL;
    double number = 0;
    char operation = '0';
    int bytes = 0;
    int check = 0;

    tempBuffer = buffer;

    while(strlen(tempBuffer)>0){
        
        check = sscanf(tempBuffer, " %f %n", &number, &bytes);
        
        if(check == 1){
            Push(&head, number);
        }
        else{  // ako nismo procitali broj, citamo operaciju
            sscanf(tempBuffer, " %c %n", &operation, &bytes);
            Calculate(&head, operation);
        }

        tempBuffer += bytes;
    }

    free(buffer);
    free(tempBuffer);

    Pop(&head, result);

    return EXIT_SUCCESS;
}

Position CreateElement(double number){

    Position new = NULL;

    new = (Position)malloc(sizeof(Stog));
    if(!new){
        perror("There was an error! Can't allocate memory!\n");
        return NULL;
    }

    new->number = number;
    new->next = NULL;

    return new;
}

int InsertAfter(Position head, Position new){

    new->next = head->next;
    head->next = new;

    return EXIT_SUCCESS;
}

int DeleteAfter(Position prev){

	Position temp = prev->next;

	if(!temp){
		return EXIT_SUCCESS;
	}

	prev->next = temp->next;
	free(temp);

	return EXIT_SUCCESS;
}

int Push(Position head, double number){

    Position new = NULL;

    new = CreateElement(number);
    if(!new){
        return -2;
    }

    InsertAfter(head, new);

    return EXIT_SUCCESS;
}

int Pop(Position head, double* result){

    Position new = NULL;

    new = head->next;
    if(!new){
        perror("There was an error! Can't allocate memory!\n");
        return -2;
    }

    *result = new->number;
    DeleteAfter(head);

    return EXIT_SUCCESS;
}

int Calculate(Position head, char operation){

    double temp1 = 0, temp2 = 0;
    double result = 0;
    int status1 = 0, status2 = 0;

    Pop(head, &temp2);
    Pop(head, &temp1);

    switch(operation){
        case '+':
            result = temp1 + temp2;
            break;
        case '-':
            result = temp1 - temp2;
            break;
        case '*':
            result = temp1 * temp2;
            break;
        case '/':
            if(temp2 == 0){
                perror("Can't divide with zero! Please fix your file and try again!\n");
                return -6;
            }
            result = temp1 / temp2;
            break;
        default:
            perror("Please only use '+', '-', '*' and '/' operations!\n");
            return -7;
    }

    Push(head, result);

    return EXIT_SUCCESS;
}