#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 1024

struct _polinom;
typedef struct _polinom* Pozicija;
typedef struct _polinom {
    int koef;
    int pot;
    Pozicija next;
}polinom;

int CitanjePodataka(Pozicija, Pozicija);
int SortiraniUnos(Pozicija, Pozicija);
Pozicija StvaranjeNovogElementa(int, int);
int IspisPolinoma(Pozicija);
int Mnozenje(Pozicija, Pozicija, Pozicija);
int Zbrajanje(Pozicija, Pozicija, Pozicija);

int main()
{
    polinom head1 = { .koef = 0, .pot = 0, .next = NULL };
    polinom head2 = { .koef = 0, .pot = 0, .next = NULL };
    polinom headSuma = { .koef = 0, .pot = 0, .next = NULL };
    polinom headProdukt = { .koef = 0, .pot = 0, .next = NULL };

    CitanjePodataka(&head1, &head2);
    
    printf("Prvi polinom: ");
    IspisPolinoma(head1.next);

    printf("Drugi polinom: ");
    IspisPolinoma(head2.next);

    Zbrajanje(head1.next, head2.next, &headSuma);
    printf("Zbroj polinoma: ");
    IspisPolinoma(headSuma.next);

    Mnozenje(head1.next, head2.next, &headProdukt);
    printf("Umnozak polinoma: ");
    IspisPolinoma(headProdukt.next);


    return 0;
}

int CitanjePodataka(Pozicija head1, Pozicija head2)
{
    FILE* fp = NULL;

    char datoteka[MAX] = { 0 };
    char buffer[MAX] = { 0 };
    char* p_buffer = &buffer;

    int status = 0;
    int koef = 0;
    int pot = 0;
    int n = 0;

    printf("Ucitajte ime datoteke: ");
    scanf(" %s", datoteka);
    fp = fopen(datoteka, "r");
    if (fp == NULL) {
        printf("Datoteka nije pronadena!");
        return -1;
    }

    fgets(buffer, MAX, fp);
    while (strlen(p_buffer) != 0) {
        status = sscanf(p_buffer, " %d %d %n", &koef, &pot, &n);
        if (status == 2) {
            Pozicija NoviElement = StvaranjeNovogElementa(koef, pot);
            SortiraniUnos(NoviElement, head1);
        }
        else {
            printf("Postoji neispravan podatak u datoteki i bit ce preskocen!\n");
            break;
        }
        p_buffer += n;
    }

    fgets(buffer, MAX, fp);
    p_buffer = &buffer;
    while (strlen(p_buffer) != 0) {
        status = sscanf(p_buffer, " %d %d %n", &koef, &pot, &n);
        if (status == 2) {
            Pozicija NoviElement = StvaranjeNovogElementa(koef, pot);
            SortiraniUnos(NoviElement, head2);
        }
        else {
            printf("Postoji neispravan podatak u datoteki i bit ce preskocen!\n");
            break;
        }
        p_buffer += n;
    }

    fclose(fp);
    return 0;
}

int SortiraniUnos(Pozicija NoviElement, Pozicija p)
{
    Pozicija prethodnik = p;
    p = p->next;

    while (p != NULL && p->pot < NoviElement->pot) {
        prethodnik = p;
        p = p->next;
    }
    if (p != NULL && NoviElement->pot == p->pot) {
        p->koef += NoviElement->koef;
        if (p->koef == 0) {
            prethodnik->next = p->next;
            free(p);
        }
    }
    else {
        NoviElement->next = p;
        prethodnik->next = NoviElement;
    }

    return 0;
}

Pozicija StvaranjeNovogElementa(int k, int p)
{
    Pozicija NoviElement = (Pozicija)malloc(sizeof(polinom));
    if (!NoviElement) {
        printf("Neuspjesno alociranje memorije!");
        return -1;
    }
    NoviElement->koef = k;
    NoviElement->pot = p;
    NoviElement->next = NULL;

    return NoviElement;
}

int Zbrajanje(Pozicija p1, Pozicija p2, Pozicija p_sum)
{
    Pozicija k = NULL;
    
    while (p1 != NULL && p2 != NULL) {
        if (p1->pot == p2->pot) {
            Pozicija novi = StvaranjeNovogElementa(p1->koef + p2->koef, p1->pot);
            SortiraniUnos(novi, p_sum);
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->pot < p2->pot) {
            Pozicija novi = StvaranjeNovogElementa(p1->koef, p1->pot);
            SortiraniUnos(novi, p_sum);
            p1 = p1->next;
        }
        else if (p1->pot > p2->pot) {
            Pozicija novi = StvaranjeNovogElementa(p2->koef, p2->pot);
            SortiraniUnos(novi, p_sum);
            p2 = p2->next;
        }
    }

    if (p2 == NULL) k = p1;
    else if (p1 == NULL) k = p2;

    while (k != NULL) {
        Pozicija novi = StvaranjeNovogElementa(k->koef, k->pot);
        SortiraniUnos(novi, p_sum);
        k = k->next;
    }
    return 0;
}

int Mnozenje(Pozicija p1, Pozicija p2, Pozicija p_umn)
{
    Pozicija head2 = p2;
    while (p1 != NULL) {
        p2 = head2;
        while (p2 != NULL) {
            Pozicija NoviElement = StvaranjeNovogElementa(p1->koef * p2->koef, p1->pot + p2->pot);
            SortiraniUnos(NoviElement, p_umn);
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    
    return 0;
}

int IspisPolinoma(Pozicija p)
{
    while (p != NULL) {
        if (p->koef == 1) printf("x^%d", p->pot);
        else printf("%dx^%d", abs(p->koef), p->pot);
        if (p->next != NULL && p->next->koef < 0) printf(" - ");
        else if (p->next != NULL && p->next->koef > 0) printf(" + ");
        else;
        p = p->next;
    }
    printf("\n");

    return 0;
}