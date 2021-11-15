#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 128
#define MAX_LEN 1024

struct _Stog;
typedef struct _Stog* Pozicija;
typedef struct _Stog {
	double broj;
	Pozicija next;
}Stog;

int ImeDatoteke(char*);
int CitanjePodatakaIzDatoteke(Pozicija, char*);
Pozicija StvaranjeNovogElementa(double);
int Push(Pozicija, Pozicija);
int Pop(Pozicija, double*);
int IzvediOperaciju(char*, Pozicija);
int Ispis(Pozicija);

int main()
{
	Stog head = { .broj = 0, .next = NULL };
	char datoteka[MAX] = { 0 };

	ImeDatoteke(&datoteka);
	CitanjePodatakaIzDatoteke(&head, &datoteka);
	Ispis(head.next);
	
	return 0;
}

int ImeDatoteke(char* datoteka)
{
	printf("Upisite ime datoteke: ");
	scanf(" %s", datoteka);

	return 0;
}

int CitanjePodatakaIzDatoteke(Pozicija head, char* datoteka)
{
	FILE* f = NULL;
	Pozicija p = head;

	char buffer[MAX_LEN] = { 0 };
	char* p_buffer;
	int status = 0;

	int n = 0;
	double broj = 0;
	char operacija[2] = { 0 };

	p_buffer = &buffer;
	f = fopen(datoteka, "r");

	if (!f) {
		perror("Datoteka nije pronadena!");
		return -1;
	}

	fgets(buffer, MAX_LEN, f);

	if (strlen(p_buffer) == 0) {
		printf("Datoteka je prazna!");
		return -1;
	}

	while (strlen(p_buffer) != 0) {
		if((status = sscanf(p_buffer, "%lf %n", &broj, &n)) == 1){
			Pozicija noviElement = StvaranjeNovogElementa(broj);
			Push(p, noviElement);
		}
		else if ((status = sscanf(p_buffer, " %s %n", operacija, &n)) == 1) {
			if (IzvediOperaciju(&operacija, p) == -1)
				return -1;
		}
		p_buffer += n;
	}

	fclose(f);
	return 0;
}

int Ispis(Pozicija p)
{
	while (p != NULL) {
		printf("%lf ", p->broj);
		p = p->next;
	}
	printf("\n");
	return 0;
}

Pozicija StvaranjeNovogElementa(double broj)
{
	Pozicija NoviElement = (Pozicija)malloc(sizeof(Stog));

	if (!NoviElement) {
		printf("Neuspjesno alociranje memorije!");
		return -1;
	}

	NoviElement->broj = broj;
	NoviElement->next = NULL;

	return NoviElement;
}

int Push(Pozicija head, Pozicija NoviElement)
{
	NoviElement->next = head->next;
	head->next = NoviElement;

	return 0;
}

int IzvediOperaciju(char* operand, Pozicija p)
{
	Pozicija rezultat = (Pozicija)malloc(sizeof(Stog));
	double broj1 = 0;
	double broj2 = 0;

	if(Pop(p, &broj1) == -1)
		return -1;
	if (Pop(p, &broj2) == -1)
		return -1;

	switch (*operand)
	{
	case '+': 
		rezultat = StvaranjeNovogElementa(broj2 + broj1);
		Push(p, rezultat);
		break;
	case '*':
		rezultat = StvaranjeNovogElementa(broj2 * broj1);
		Push(p, rezultat);
		break;
	default:
		break;
	}
}

int Pop(Pozicija head, double* br)
{
	Pozicija temp = NULL;

	if (head->next == NULL) {
		printf("Neispravan postfix u datoteki.\n");
		return -1;
	}

	*br = head->next->broj;
	temp = head->next;
	head->next = temp->next;
	free(temp);

	return 0;
}