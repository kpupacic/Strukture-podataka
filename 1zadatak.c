#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct student{
    char ime[MAX_LINE];
    char prezime[MAX_LINE];
    int bodovi;
} _student;

int BrojacStudenata(FILE* dat);
_student* CitanjePodataka(_student*, int, FILE*);

int main(){

    FILE *dat = NULL;
    _student *studenti = NULL;
    int i, n;

    dat = fopen("rezultati.txt", "r");

    n = BrojacStudenata(dat);
    printf("Broj studenata u datoteci: %d", n);
    studenti = (_student*)malloc(n*sizeof(_student));

    rewind(dat);
    CitanjePodataka(studenti, n, dat);
    
    fclose(dat);

    return 0;
}

int BrojacStudenata(FILE* dat){
    int brojac = 0;
    char buffer[MAX_LINE] = {0};

    if(!dat){
        printf("Greska prilikom otvaranja datoteke!");
        return -1;
    }
    while(!feof(dat)){
        fgets(buffer, MAX_LINE, dat);
        brojac++;
    }

    return brojac;
};

_student* CitanjePodataka(_student *stud, int n, FILE* dat){

    int i;
    int maxBrojBodova = 0;
    float *relativniBodovi = NULL;

    relativniBodovi = (float*)malloc(n*sizeof(float));

    for(i=0; i<n; i++){
        fscanf(dat, " %s %s %d", stud[i].ime, stud[i].prezime, &stud[i].bodovi);
        if(maxBrojBodova<stud[i].bodovi){
            maxBrojBodova = stud[i].bodovi;
        }
    }

    for(i=0; i<n; i++){
        relativniBodovi[i] = (double)stud[i].bodovi/maxBrojBodova*100;
        printf("\n%s %s %d %.2f%%", stud[i].ime, stud[i].prezime, stud[i].bodovi, relativniBodovi[i]);
    }

    return stud;
};