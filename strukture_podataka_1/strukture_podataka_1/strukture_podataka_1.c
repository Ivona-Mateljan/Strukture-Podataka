#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#pragma warning (disable:4996)

#define MAX 128

typedef struct _studenti {
	char ime[MAX];
	char prezime[MAX];
	double ocjena;
}studenti;

int CitanjeBrojaRedaka();
int UcitavanjePodataka(int, studenti**);
int IspisPodataka(int, studenti*);

int main()
{
	int broj_studenata = 0;
	studenti* student = NULL;
	
	broj_studenata = CitanjeBrojaRedaka();
	if (broj_studenata != 0) {
		UcitavanjePodataka(broj_studenata, &student);
		IspisPodataka(broj_studenata, student);
	}

	return 0;
}

int CitanjeBrojaRedaka()
{
	int br = 0;
	FILE* datoteka = NULL;
	
	datoteka = fopen("studenti.txt", "r");
	
	if (datoteka == NULL) {
		printf("Datoteka nije pronadena!");
		return -1;
	}

	while (!feof(datoteka)) {
		char buffer[MAX] = { '\0' };
		fgets(buffer, MAX, datoteka);
		if(buffer[0] != '\0')
			br++;
	}

	fclose(datoteka);

	if (br == 0) {
		printf("Datoteka je prazna!");
		return 0;
	}
	else return br;

}

int UcitavanjePodataka(int broj_studenata, studenti** student)
{
	FILE* datoteka = NULL;
	int i = 0;

	datoteka = fopen("studenti.txt", "r");

	if (datoteka == NULL) {
		printf("Datoteka nije pronadena!");
		return -1;
	}

	*student = (studenti*)malloc(broj_studenata * sizeof(studenti));

	for(i = 0; i < broj_studenata; i++) {
		fscanf(datoteka, " %s %s %lf", (*student + i)->ime, (*student + i)->prezime, &(*student + i)->ocjena);
	}

	fclose(datoteka);

	return 0;
}
int IspisPodataka(int broj_studenata, studenti* student)
{
	double relativan_br_bodova = 0, br_bodova = 0, max_br_bodova = 0;
	int i = 0;

	for (i = 0; i < broj_studenata; i++) {
		if ((student + i)->ocjena > max_br_bodova)
			max_br_bodova = (student + i)->ocjena;
	}

	printf("IME PREZIME APSOLUTNI RELATIVNI\n");

	for (i = 0; i < broj_studenata; i++) {
		printf("%s  %s  %.2lf\t%.2lf\n",
			(student + i)->ime,
			(student + i)->prezime,
			(student + i)->ocjena,
			(student + i)->ocjena / max_br_bodova * 100);
	}
	
	return 0;
}