#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

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
int InsertAfter(Position prevPerson, Position newPerson);
int PrependList(Position head, char* name, char* surname, int birth);     // A)     // prepend - add to the beginning
int PrintList(Position head, Position firstPerson);                                      // B)
int AppendList(Position head, char* name, char* surname, int birth);      // C)     // append - add to the end
Position FindSurname(Position head, char* surname);                       // D)
Position FindBefore(Position head, Position person);
int DeletePerson(Position head, Position deletedPerson);                  // E)

int main(){

    Person Head = {.name = {0}, .surname = {0}, .birth = 0, .next = NULL};
    Position pHead = &Head;
    Person Leia, Han, Yoda;
    Position pLeia = &Leia, pHan = &Han, pYoda = &Yoda;

    pLeia = CreatePerson("Leia", "Organa", 1111);
    pHan = CreatePerson("Han", "Solo", 1105);
    pYoda = CreatePerson("Yoda", "Gremlin", 0001);

    InsertAfter(pHead, pLeia);
    InsertAfter(pLeia, pHan);
    InsertAfter(pHan, pYoda);

    // prvi ispis liste
    PrintList(pHead, pHead);

    PrependList(pHead, "Luke", "Skywalker", 1111);

    AppendList(pHead, "Obi", "Wan Kenobi", 950);
    
    printf("\n\nIspis liste nakon unosa nove osobe na pocetak i na kraj liste:");
    PrintList(pHead, pHead);

    printf("\n\nPostoji li osoba prezimena Gremlin?\nPronadena osoba: %s %s\n", (FindSurname(pHead, "Gremlin"))->name, (FindSurname(pHead, "Gremlin"))->surname);

    DeletePerson(pHead, pYoda);

    printf("\n\nIspis liste nakon brisanja osobe:");
    PrintList(pHead, pHead);

    return 0;
}

Position CreatePerson(char* name, char* surname, int birth){

    Position newPerson = NULL;

    newPerson = (Position)malloc(sizeof(Person));
    if(!newPerson){
        perror("Alokacija memorije nije moguca!\n");
        return -1;
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
        return -1;
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

int PrintList(Position head, Position firstPerson){

    Position temp = firstPerson;
    
    // u slucaju da ne znamo tko se nalazi na prvoj poziciji, a zelimo napraviti ispis od pocetka liste: "head, head"
    // ako zelimo ispisati listu od neke odredene osobe: "head, zeljenaOsoba"
    if(temp == head){
        temp = head->next;
    }

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