#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50
#define MAX_LINE 1024

struct _Person;
typedef struct _Person* Position;
typedef struct _Person{
    char name[MAX];
    char surname[MAX];
    int birth;
    Position next;
} Person;

Position CreatePerson(char* name, char* surname, int birth);
Position FindLast(Position head);
int InsertAfter(Position prevPerson, Position newPerson);                       // 3.A)
int PrependList(Position head, char* name, char* surname, int birth);           // 2.A) (prepend - add to the beginning)
int PrintList(Position head);                                                   // 2.B)
int AppendList(Position head, char* name, char* surname, int birth);            // 2.C) (append - add to the end)
Position FindSurname(Position head, char* surname);                             // 2.D)
Position FindBefore(Position head, Position person);
int DeletePerson(Position head, Position deletedPerson);                        // 2.E)
int InsertBefore(Position head, Position newPerson, Position nextPerson);       // 3.B)
int SortBySurname(Position head);                                               // 3.C) (sort an existing list)
int CreateAndSortBySurname(Position head, char* name, char* surname, int birth);// 3.C) (create a new person and sort them immediately)
int CopyToDocument(FILE *f, Position head);                                     // 3.D)
int CopyFromDocument(FILE *f);                                                  // 3.E)

int main(){

    Person Head = {.name = {0}, .surname = {0}, .birth = 0, .next = NULL};
    Position pHead = &Head;
    Person Head2 = {.name = {0}, .surname = {0}, .birth = 0, .next = NULL};
    Position pHead2 = &Head2;
    Person Leia, Han, Yoda;
    Position pLeia = &Leia, pHan = &Han, pYoda = &Yoda;
    Person Anakin, Padme;
    Position pAnakin = &Anakin, pPadme = &Padme;
    FILE *f = NULL;

    // nedostaje menu (mozda dodati switch-case?)
    pLeia = CreatePerson("Leia", "Organa", 1111);
    pHan = CreatePerson("Han", "Solo", 1105);
    pYoda = CreatePerson("Yoda", "Gremlin", 0001);
    pAnakin = CreatePerson("Anakin", "Skywalker", 960);
    pPadme = CreatePerson("Padme", "Amidala", 950);

    // umetanje osobe nakon neke druge osobe
    InsertAfter(pHead, pLeia);
    InsertAfter(pLeia, pHan);
    InsertAfter(pHan, pYoda);

    // prvi ispis liste
    PrintList(pHead);

    // umetanje osobe na pocetak liste
    PrependList(pHead, "Luke", "Skywalker", 1111);
    // umetanje osobe na kraj liste
    AppendList(pHead, "Obi", "Wan Kenobi", 950);
    printf("\n\nIspis liste nakon unosa nove osobe na pocetak i na kraj liste:");
    PrintList(pHead);

    // trazenje po prezimenu
    printf("\n\nPostoji li osoba prezimena Gremlin?\nPronadena osoba: %s %s\n", (FindSurname(pHead, "Gremlin"))->name, (FindSurname(pHead, "Gremlin"))->surname);
    // brisanje postojece osobe
    DeletePerson(pHead, pYoda);
    printf("\n\nIspis liste nakon brisanja osobe:");
    PrintList(pHead);

    // umetanje osobe prije neke druge osobe
    InsertBefore(pHead, pAnakin, FindSurname(pHead, "Wan Kenobi"));
    // (ponovno) umetanje osobe nakon neke druge osobe, zbog 3. zadatka
    InsertAfter(pAnakin, pPadme);
    printf("\n\nKonacan ispis svih osoba na listi:");
    PrintList(pHead);

    // sortiranje liste po prezimenu
    SortBySurname(pHead);

    // upisivanje liste (poredane po prezimenu) u datoteku
    CopyToDocument(f, pHead);
    // ispisivanje datoteke na standardni izlaz
    printf("\n\nOsobe poredane po prezimenu:\n");
    CopyFromDocument(f);

    // za testiranje funkcije koja sortira pri samom unosu osobe, stvaram novu listu (dummy: Head2)
    CreateAndSortBySurname(pHead2, "Rey", "Skywalker", 1145);
    CreateAndSortBySurname(pHead2, "Finn", "Stormtrooper", 1150);
    CreateAndSortBySurname(pHead2, "Poe", "Dameron", 1155);
    PrintList(pHead2);

    return 0;
}

Position CreatePerson(char* name, char* surname, int birth){

    Position newPerson = NULL;

    newPerson = (Position)malloc(sizeof(Person));
    if(!newPerson){
        perror("Alokacija memorije nije moguca!\n");
        return NULL;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birth = birth;
    newPerson->next = NULL;

    return newPerson;
}

Position FindLast(Position head){

    Position lastPerson = head;

    if(!lastPerson){
        return NULL;
    }

    while(lastPerson->next){
        lastPerson = lastPerson->next;
    }

    return lastPerson;
}

int InsertAfter(Position prevPerson, Position newPerson){

    if(!prevPerson || !newPerson){
        return -1;
    }

    newPerson->next = prevPerson->next;
    prevPerson->next = newPerson;

    return 0;
}

int PrependList(Position head, char* name, char* surname, int birth){

    Position newPerson = NULL;

    newPerson = CreatePerson(name, surname, birth);
    if(!newPerson){
        perror("Nije moguce ucitati novog korisnika!");
        return -1;
    }

    InsertAfter(head, newPerson);

    return 0;
}

int PrintList(Position head){

    Position temp = head->next;

    while(temp){
        printf("\n%s %s, %d.", temp->name, temp->surname, temp->birth);
        temp = temp->next;
    }

    return 0;
}

int AppendList(Position head, char* name, char* surname, int birth){

    Position newPerson = NULL;
    Position lastPerson = NULL;

    newPerson = CreatePerson(name, surname, birth);
    if(!newPerson){
        perror("Nije moguce ucitati novog korisnika!");
        return -1;
    }

    lastPerson = FindLast(head);

    lastPerson->next = newPerson;

    return 0;
}

Position FindSurname(Position head, char* surname){

    Position temp = head;

    while(temp){
        if(strcmp(temp->surname, surname)==0){
            return temp;
        }
        temp = temp->next;
    }

    return temp;
}

Position FindBefore(Position head, Position Person){

    Position prevPerson = NULL;
    Position temp = head;

    while(temp!=Person){
        prevPerson = temp;
        temp = temp->next;
    }

    return prevPerson;
}

int DeletePerson(Position head, Position deletedPerson){

    Position prevPerson = NULL;

    prevPerson = FindBefore(head, deletedPerson);

    prevPerson->next = deletedPerson->next;
    free(deletedPerson);

    return 0;
}

int InsertBefore(Position head, Position newPerson, Position nextPerson){

    Position temp = NULL;
    temp = FindBefore(head, nextPerson);
    InsertAfter(temp, newPerson);

    return 0;
}

int SortBySurname(Position head){

    Position temp1 = NULL, temp2 = NULL, temp3 = NULL, end = NULL;

    while(head->next!=end){
        temp1 = head;
        temp2 = head->next;
        while(temp2->next!=end){
            if(strcmp(temp2->surname, temp2->next->surname)>0){
                temp3 = temp2->next;
                temp1->next = temp3;
                temp2->next = temp3->next;
                temp3->next = temp2;
                temp2 = temp3;
            }
            temp1 = temp2;
            temp2 = temp2->next;
        }
        end = temp2;
    }

    return 0;
}

int CopyToDocument(FILE *f, Position head){

    Position temp = head->next;
    
    f = fopen("sortbysurname.txt", "w");

    while(temp){
        fprintf(f, "%s %s, %d.\n", temp->name, temp->surname, temp->birth);
        temp = temp->next;
    }

    fclose(f);

    return 0;
}

int CopyFromDocument(FILE *f){

    char buffer[MAX_LINE] = {0};

    f = fopen("sortbysurname.txt", "r");

    if(!f){
        printf("Problem pri otvaranju datoteke!");
        return -1;
    }

    while(!feof(f)){
        fgets(buffer, MAX_LINE, f);
        printf("%s", buffer);
    }

    fclose(f);

    return 0;
}

int CreateAndSortBySurname(Position head, char* name, char* surname, int birth){

    Position temp = head;
    Position new = NULL;

    new = CreatePerson(name, surname, birth);

    for(temp; temp->next!=NULL; temp=temp->next){
        if(strcmp(temp->next->surname, new->surname)>0) break;
        if(strcmp(temp->next->surname, new->surname)==0){
            if(strcmp(temp->next->name, new->name)>0) break;
            if(strcmp(temp->next->name, new->name)==0){
                if(temp->next->birth > new->birth) break;
            }
        }
    }
    
    InsertAfter(temp, new);

    return 0;
}
