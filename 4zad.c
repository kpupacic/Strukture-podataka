#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define MAX_LINE 1024

struct _Poly;
typedef struct _Poly* Position;
typedef struct _Poly{
    int coef;
    int expo;
    Position next;
} Poly;

int ReadFile(FILE *f, Position one, Position two);
int SortedAdd(Position head, int coef, int expo);
int PrintList(Position head);                                   // ispisuje polinom u obliku "koef = [broj], ekspo = [broj]"
int PrintPolynomial(Position head);                             // ispisuje polinom u matematickom obliku
int Zbroj(Position one, Position two, Position result);
int Umnozak(Position one, Position two, Position result);
int SimplifyPoly(Position head);                                // zbraja koeficijente istog eksponenta

int main(){

    Poly head1 = {.coef = 0, .expo = 0, .next = NULL};
    Poly head2 = {.coef = 0, .expo = 0, .next = NULL};
    Poly zbroj = {.coef = 0, .expo = 0, .next = NULL};
    Poly umnozak = {.coef = 0, .expo = 0, .next = NULL};
    FILE *f = NULL;

    ReadFile(f, &head1, &head2);

    printf("\nIspis prvog polinoma:\n");
    PrintList(&head1);

    printf("\nIspis drugog polinoma:\n");
    PrintList(&head2);

    Zbroj(&head1, &head2, &zbroj);
    Umnozak(&head1, &head2, &umnozak);

    printf("\nZbroj polinoma:\n");
    SimplifyPoly(&zbroj);
    PrintPolynomial(&zbroj);

    printf("\nUmnozak polinoma:\n");
    PrintPolynomial(&umnozak);  // ispis cijelog polinoma, jer funkcija SimplifyPoly ne radi za umnozak iz nekog razloga?? (popravi)
    //SimplifyPoly(&umnozak);
    //PrintPolynomial(&umnozak);

    return 0;
}

int ReadFile(FILE *f, Position one, Position two){

    char buffer[MAX_LINE] = {0};
    char *tempBuffer;
    int n = 0;
    int tempCoef, tempExpo, bytes;
    Position new = NULL;
    Position temp = NULL, prev = NULL;

    f = fopen("polinomi.txt", "r");

    if(!f){
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    // citanje prve linije datoteke
    fgets(buffer, MAX_LINE, f);
    tempBuffer = buffer;
    while(strlen(tempBuffer) > 0){
        
        n = sscanf(tempBuffer, " %d %d %n", &tempCoef, &tempExpo, &bytes);
        
        // ako n!=2 znaci da je u bufferu neparano brojeva, pa ignoriramo višak
        if(n==2){
            temp = one->next;
            prev = one;
            SortedAdd(one, tempCoef, tempExpo);
        }
        
        // ovako prolazimo kroz buffer dok mu ne iscitamo sve brojeve
        tempBuffer = tempBuffer + bytes;
    }

    // citanje druge linije datoteke
    fgets(buffer, MAX_LINE, f);
    tempBuffer = buffer;
    while(strlen(tempBuffer) > 0){
        
        n = sscanf(tempBuffer, " %d %d %n", &tempCoef, &tempExpo, &bytes);
        
        if(n==2){
            temp = two->next;
            prev = two;
            SortedAdd(two, tempCoef, tempExpo);
        }
        
        tempBuffer = tempBuffer + bytes;
    }

    fclose(f);

    return EXIT_SUCCESS;
}

int SortedAdd(Position head, int coef, int expo){

    Position temp = NULL;
    Position prev = NULL;
    Position new = NULL;

    temp = head->next;
    prev = head;

    while(temp!=NULL && temp->expo > expo){
        prev = temp;
        temp = temp->next;
    }
    new = (Position)malloc(sizeof(Poly));
    new->coef = coef;
    new->expo = expo;
    new->next = NULL;

    new->next = prev->next;
    prev->next = new;

    return EXIT_SUCCESS;
}

int PrintList(Position head){

    while(head->next!=NULL){
        printf("Koef: %d, ekspo: %d\n", head->next->coef, head->next->expo);
        head = head->next;
    }

    return EXIT_SUCCESS;
}

int PrintPolynomial(Position head){

    Position temp = head->next;

    while(temp!=NULL){
        
        printf("%dx^%d ", temp->coef, temp->expo);
        
        if(temp->next!=NULL){
            printf("+ ");
        }

        temp = temp->next;
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}

int Zbroj(Position one, Position two, Position result){

    Position temp1 = NULL, temp2 = NULL, temp3 = NULL;
    Position new = NULL;
    int tempCoef = 0, tempExpo = 0;

    temp1 = one->next;
    temp2 = two->next;

    while(temp1!=NULL && temp2!=NULL){

        new = (Position)malloc(sizeof(Poly));
        
        if(temp1->expo == temp2->expo){
            new->coef = temp1->coef + temp2->coef;
            new->expo = temp1->expo;
            SortedAdd(result, new->coef, new->expo);
            temp1 = temp1->next;
            temp2 = temp2->next;
        }

        else if(temp1->expo > temp2->expo){
            new->coef = temp2->coef;
            new->expo = temp2->expo;
            SortedAdd(result, new->coef, new->expo);
            temp2 = temp2->next;
        }

        else{
            new->coef = temp1->coef;
            new->expo = temp1->expo;
            SortedAdd(result, new->coef, new->expo);
            temp1 = temp1->next;
        }

    }

    // ako smo dosli do kraja prve liste, prepisujemo ostatak iz druge liste
    if(temp1 == NULL){
        temp3 = temp2;
    }
    // ako smo dosli do kraja druge liste, prepisujemo ostatak iz prve liste
    else if(temp2 == NULL){
        temp3 = temp1;
    }

    while(temp3!=NULL){
        SortedAdd(result, temp3->coef, temp3->expo);
        temp3 = temp3->next;
    }

    return EXIT_SUCCESS;
}

int Umnozak(Position one, Position two, Position result){

    Position temp1 = NULL, temp2 = NULL;
    int tempCoef = 0, tempExpo = 0;

    temp1 = one->next;

    while(temp1!=NULL){
        temp2 = two->next;
        while(temp2!=NULL){
            
            tempCoef = temp1->coef * temp2->coef;
            tempExpo = temp1->expo + temp2->expo;

            SortedAdd(result, tempCoef, tempExpo);

            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }

    return EXIT_SUCCESS;
}

int SimplifyPoly(Position head){

    Position prev = NULL, temp = NULL;

    prev = head;

    while(prev->next!=NULL){
        temp = prev->next;
        if(prev->expo == temp->expo){
            prev->coef = prev->coef + temp->coef;
            prev->next = temp->next;
            free(temp);
        }
        prev = prev->next;
    }

    return EXIT_SUCCESS;
}