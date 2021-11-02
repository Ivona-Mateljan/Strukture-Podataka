#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#pragma warning (disable:4996)

#define MAX (128)

struct _osoba;
typedef struct _osoba* pozicija;

typedef struct _osoba {
	char ime[MAX];
	char prezime[MAX];
	int godine;
	pozicija next;
}osoba;

int CitanjePodatakaIzDatoteke(pozicija, char*);
pozicija StvoriNovuOsobu(char*, char*, int);
int UcitavanjeNaKraj(char*, char*, int, pozicija);
int UcitavanjeNaPocetak(char*, char*, int, pozicija);
int Ispis(pozicija);
pozicija PronalazenjeElementaPoPrezimenu(char*, pozicija);
pozicija PronalazenjePrethodnika(char*, pozicija);
int BrisanjeElementa(char*, pozicija);
int Zamjena(pozicija, pozicija);
pozicija PronadiZadnji(pozicija);
int SortirajPoPrezimenu(pozicija); 
int JeliListaPrazna(pozicija);
int UpisuDatoteku(pozicija, char*);
int UcitavanjeIspred(char*, char*, char*, int, pozicija);
int UcitavanjeIza(char*, char*, char*, int, pozicija);

int main()
{
	osoba head = { .next = NULL, .ime = {0}, .prezime = {0}, .godine = 0};
	char datoteka[MAX] = { 0 };
	char ime[MAX] = { 0 };
	char prezime[MAX] = { 0 };
	char prezime1[MAX] = { 0 };
	int godine = 0;
	int izbor = -1;

	printf("Upisite ime datoteke iz koje zelite procitati podatke: ");
	scanf(" %s", datoteka);
	CitanjePodatakaIzDatoteke(&head, &datoteka);

	while(izbor != 0){
		if(!JeliListaPrazna(&head)){
			printf("OPCIJE:\n1.\tIspis liste\n2.\tDodati novi element na kraj liste\n3.\tDodati novi element na pocetak liste\n"
				"4.\tIzbrisati element\n5.\tSortiraj listu po prezimenu\n"
				"6.\tUpisati novi element ispred nekog clana\n7.\tUpisati novi element iza nekog clana\n8.\tUpisati podatke u datoteku\n"
				"0.\tZavrsiti program\n");

			printf("Odaberite sto zelite ucinit s listom: ");
			scanf("%d", &izbor);

			switch (izbor)
			{	
			case 1:
				Ispis(head.next);
				break;

			case 2:
				printf("Upisite podatke novog clana: ");
				scanf(" %s %s %d", ime, prezime, &godine);
				UcitavanjeNaKraj(&ime, &prezime, godine, &head);
				Ispis(head.next);
				break;
			
			case 3:
				printf("Upisite podatke novog clana: ");
				scanf(" %s %s %d", ime, prezime, &godine);
				UcitavanjeNaPocetak(&ime, &prezime, godine, &head);
				Ispis(head.next);
				break;
			
			case 4:
				printf("Upisite prezime clana koji zelite izbrisati: ");
				scanf(" %s", prezime);
				BrisanjeElementa(&prezime, &head);
				Ispis(head.next);
				break;
			
			case 5:
				SortirajPoPrezimenu(&head);
				Ispis(head.next);
				break;
			
			case 6:
				printf("Upisite prezime clana ispred kojeg zelite upisati novi clan: ");
				scanf(" %s", prezime1);
				if (PronalazenjeElementaPoPrezimenu(&prezime1, &head) == NULL) {
					break;
				}
				printf("Upisite podatke novog clana: ");
				scanf(" %s %s %d", ime, prezime, &godine);
				UcitavanjeIspred(&prezime1, &ime, &prezime, godine, &head);
				Ispis(head.next);
				break;
			
			case 7:
				printf("Upisite prezime clana iza kojeg zelite upisati novi clan: ");
				scanf(" %s", prezime1);
				if (PronalazenjeElementaPoPrezimenu(&prezime1, &head) == NULL) {
					break;
				}
				printf("Upisite podatke novog clana: ");
				scanf(" %s %s %d", ime, prezime, &godine);
				UcitavanjeIza(&prezime1, &ime, &prezime, godine, &head);
				Ispis(head.next);
				break;
			
			case 8:
				printf("Upisite ime datoteke u koju zelite upisati podatke: ");
				scanf(" %s", datoteka);
				UpisuDatoteku(head.next, &datoteka);
				break;

			case 0:
				printf("Izlaz iz programa!");
				break;
			
			default: 
				printf("Niste odabrali ni jednu od ponudenih opcija.\n");
				break;
			
			}
		}

		else
		{
			printf("Lista je trenutno prazna!\nOdaberite 1 za ponovni unos datoteke, 2 za unos elemenata u listu ili 0 za zavrsetak programa.\n");
			scanf("%d", &izbor);

			switch (izbor)
			{
			case 1:
				printf("Upisite ime datoteke iz koje zelite procitati podatke: ");
				scanf(" %s", datoteka);
				CitanjePodatakaIzDatoteke(&head, &datoteka);
				break;

			case 2:
				UcitavanjeNaKraj(&ime, &prezime, godine, &head);
				Ispis(head.next);
				break;
			
			case 0:
				printf("Izlaz iz programa!");
				break;
			
			default:
				printf("Niste odabrali ni jednu od ponudenih opcija.\n");
				break;
			}
		}
	}
	
	return 0;
}


int CitanjePodatakaIzDatoteke(pozicija p, char* dat)
{
	int br = 0;
	FILE* datoteka = NULL;
	char temp_ime[MAX] = { '\0' };
	char temp_prezime[MAX] = { '\0' };
	int temp_godine = 0;
	pozicija nova_osoba = NULL;

	datoteka = fopen(dat, "r");

	if (datoteka == NULL) {
		perror("Datoteka nije pronadena!");
		return -1;
	}

	while (!feof(datoteka)) {
		char buffer[1024] = { '\0' };
		fgets(buffer, 1024, datoteka);
		if (sscanf(buffer, " %s %s %d", temp_ime, temp_prezime, &temp_godine) == 3) {
			UcitavanjeNaKraj(&temp_ime, &temp_prezime, temp_godine, p);
			br++;
		}
	}

	fclose(datoteka);

	if (br == 0) {
		printf("Datoteka je prazna ili nema ispravnih podataka!");
		return 1;
	}
	else return 0;
}

pozicija StvoriNovuOsobu(char* ime, char* prezime, int godine)
{
	pozicija nova_osoba = NULL;

	nova_osoba = (pozicija)malloc(sizeof(osoba));

	if (!nova_osoba) {
		perror("Neuspjesno alociranje memorije!");
		return NULL;
	}

	strcpy(nova_osoba->ime, ime);
	strcpy(nova_osoba->prezime, prezime);
	nova_osoba->godine = godine;

	return nova_osoba;
}

int UcitavanjeNaKraj(char* ime, char* prezime, int godine, pozicija p)
{
	while (p->next != NULL)
		p = p->next;
	UcitavanjeNaPocetak(ime, prezime, godine, p);
	
	return 0;
}

int UcitavanjeNaPocetak(char* ime, char* prezime, int godine, pozicija p)
{
	pozicija nova_osoba = NULL;
	
	nova_osoba = StvoriNovuOsobu(ime, prezime, godine);

	nova_osoba->next = p->next;
	p->next = nova_osoba;

	return 0;
}

int Ispis(pozicija p)
{
	printf("\nTRENUTNA LISTA:\n");
	while (p != NULL) {
		printf("%s %s %d\n", p->ime, p->prezime, p->godine);
		p = p->next;
	}
	printf("\n");
	
	return 0;
}

int BrisanjeElementa(char* prezime, pozicija p)
{
	pozicija temp = NULL;
	pozicija pozicija_prezime = PronalazenjeElementaPoPrezimenu(prezime, p);
	pozicija pozicija_prethodnika = PronalazenjePrethodnika(prezime, p);
	if (pozicija_prezime == NULL) printf("Nismo uspjeli pronaci element!");
	else {
		temp = pozicija_prethodnika->next;
		pozicija_prethodnika->next = pozicija_prethodnika->next->next;
		free(temp);
	}
	return 0;
}

pozicija PronalazenjePrethodnika(char* prezime_sljedbenika, pozicija p)
{
	while (p->next != NULL && strcmp(p->next->prezime, prezime_sljedbenika) != 0) 
		p = p->next;

	if (p->next == NULL) return NULL;
	else return p;
}

pozicija PronalazenjeElementaPoPrezimenu(char *prezime, pozicija p)
{
	while (p != NULL && strcmp(p->prezime, prezime) != 0) {
		p = p->next;
	}
	if (p == NULL) {
		printf("Upisani element nije u listi!\n");  
		return NULL;
	}
	else return p;
}

int SortirajPoPrezimenu(pozicija head)
{
	pozicija zadnji = NULL;
	pozicija temp = NULL;
	pozicija prvi_stvarni = head->next;

	while (prvi_stvarni != zadnji) {
		pozicija p = head;
		pozicija i = head->next;
		pozicija j = head->next->next;
		while (j != zadnji) {
			if (strcmp(i->prezime, j->prezime) > 0){
				Zamjena(i, j, p);
				temp = j;
				j = i;
				i = temp;
			}
			p = p->next;
			i = i->next;
			j = j->next;
		}
		zadnji = i;
	}
	return 0;
}

pozicija PronadiZadnji(pozicija p)
{
	while (p->next != NULL)
		p = p->next;

	return p;
}


int Zamjena(pozicija clan1, pozicija clan2, pozicija p)
{
	clan1->next = clan2->next;
	clan2->next = clan1;
	p->next = clan2;

	return 0;
}

int JeliListaPrazna(pozicija head)
{
	if (head->next == NULL) 
		return 1;
	else return 0;
}

int UcitavanjeIspred(char* prezime_ispred, char* ime, char *prezime, int godine, pozicija p)
{
	pozicija nova_osoba = StvoriNovuOsobu(ime, prezime, godine);
	pozicija prethodnik = PronalazenjePrethodnika(prezime_ispred, p);

	nova_osoba->next = prethodnik->next;
	prethodnik->next = nova_osoba;

	return 0;
}

int UcitavanjeIza(char* prezime_ispred, char* ime, char* prezime, int godine, pozicija p)
{
	pozicija nova_osoba = StvoriNovuOsobu(ime, prezime, godine);
	pozicija osoba_ispred = PronalazenjeElementaPoPrezimenu(prezime_ispred, p);

	nova_osoba->next = osoba_ispred->next;
	osoba_ispred->next = nova_osoba;
	return 0;
}

int UpisuDatoteku(pozicija p, char* dat)
{
	FILE* datoteka = NULL;
	char izbor[3] = { 0 };

	datoteka = fopen(dat, "w");

	if (datoteka == NULL) {
		printf("Datoteka nije pronadena!\n");
		return -1;
	}

	while (p != NULL) {
		fprintf(datoteka, "%s %s %d\n", p->ime, p->prezime, p->godine);
		p = p->next;
	}

	fclose(datoteka);
	
	return 0;
}

