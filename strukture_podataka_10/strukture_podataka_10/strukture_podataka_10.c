#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LEN 128
#define MAX 1024

struct _Country;
struct _City;
typedef struct _Country* Position;
typedef struct _City* Child;
typedef struct _City* Pointer;

typedef struct _Country {
	char name[MAX_LEN];
	Pointer cities_tree;
	Position next_country;
}Country;

typedef struct _City {
	char name[MAX_LEN];
	int population;
	Child Left;
	Child Right;
}City;

int OpenFile(char*, Position);
int ReadFromFile(char*, Position);
Position CreateNewCountry(char*);
Pointer CreateNewCity(char*, int);
int AddNewSorted(Position, Position);
Pointer AddNewCity(Pointer, Pointer);
int PrintList(Position);
int PrintCities(Pointer);
int UserInput(Position);
int FindMorePopulated(Pointer, int);

int main()
{
	Country head = { .name = {0}, .next_country = NULL, .cities_tree = NULL };
	char file[] = "drzave.txt";
	char* fp = &file;

	if(OpenFile(fp, &head) == EXIT_FAILURE)
		return EXIT_FAILURE;

	PrintList(head.next_country);

	
	UserInput(head.next_country);

	return EXIT_SUCCESS;
}

int OpenFile(char* file, Position head)
{
	FILE* fp = NULL;
	fp = fopen(file, "r");
	if (fp == NULL) {
		perror("File not found!");
		return EXIT_FAILURE;
	}
	else if (ReadFromFile(file, head) == EXIT_FAILURE) {
			fclose(fp);
			return EXIT_FAILURE;
		}
	
	fclose(fp);

	return EXIT_SUCCESS;
}

int ReadFromFile(char* filename, Position head)
{
	char buffer[MAX] = { 0 };
	char temp_name[MAX_LEN] = { 0 };
	char file_name[MAX_LEN] = { 0 };
	char name[MAX_LEN] = { 0 };
	int population = 0;
	Position new_country = NULL;
	Pointer new_city = NULL;

	FILE* file = NULL;
	file = fopen(filename, "r");
	if (file == NULL) {
		perror("File not found!");
		return EXIT_FAILURE;
	}

	while (!feof(file)) {
		fgets(buffer, MAX, file);
		char* p_buffer = &buffer;

		//jesmo li u datoteki s drzavama ili s gradovima
		char* last_char = &buffer[strlen(p_buffer) - 2]; // - 2 zbog \n
		if ((int) *last_char >=65 && (int) *last_char <= 122) { //zadnji znak je slovo
			//datoteka s drzavama
			if (sscanf(p_buffer, "%s %s", temp_name, file_name) == 2) {
				char country_name[MAX_LEN] = { 0 };

				//jeli zapravo datoteka s drzavavama ili neispravna datoteka s gradovima
				//ako smo u datoteki s drzavama, ima li ona na kraju .txt
				char* last_five = &buffer[strlen(buffer) - 5]; 
				//printf("%s", last_five + 1);
				if (strcmp(last_five, ".txt\n") != 0 && strcmp(last_five + 1, ".txt") != 0) {
					printf("%s file is faulty! Please check info!", filename);
					return EXIT_FAILURE;
				}

				//drzava ima vise rijeci?
				while (strlen(p_buffer) != 0) {
					sscanf(p_buffer, "%s %s", temp_name, file_name);
					strcat(country_name, temp_name);
					char* last_four = &file_name[strlen(file_name) - 4];

					if (strcmp(last_four, ".txt") != 0) { 
						
						strcat(country_name, " ");
						p_buffer += (strlen(temp_name) + 1) * sizeof(char); //+ 1 zbog razmaka
					}
					else if (strcmp(p_buffer + strlen(p_buffer) - 1, "\n") != 0) 
						p_buffer += (strlen(temp_name) + strlen(file_name) + 1) * sizeof(char);
					else p_buffer += (strlen(temp_name) + strlen(file_name) + 2) * sizeof(char); //+ 2 zbog \n
				}

				char* name = &country_name;
				char* file_cities = &file_name;
				new_country = CreateNewCountry(name);
				if (new_country == NULL) return EXIT_FAILURE;
				else AddNewSorted(new_country, head);
				if (OpenFile(file_cities, new_country) == EXIT_FAILURE)
					return EXIT_FAILURE;
			}
		}
		else {
			char city_name[MAX_LEN] = { 0 };
			if (sscanf(p_buffer, "%s %d", temp_name, &population) != 2) {
				//Grad ima vise rijeci
				while (sscanf(p_buffer, "%s %d", temp_name, &population) != 2) {
					strcat(city_name, temp_name);
					strcat(city_name, " ");
					p_buffer += (strlen(temp_name) + 1) * sizeof(char);
					if (strlen(p_buffer) == 0) {
						printf("%s file is faulty! Please check info!", filename);
						return EXIT_FAILURE;
					}
				}
				strcat(city_name, temp_name);
			}
			else strcat(city_name, temp_name);
			char* name = &city_name;
			new_city = CreateNewCity(name, population);
			if (new_city == NULL) return EXIT_FAILURE;
			else head->cities_tree = AddNewCity(new_city, head->cities_tree);
		}	
	}
	fclose(file);
	return EXIT_SUCCESS;
}

Position CreateNewCountry(char* country_name)
{
	Position new_country = NULL;
	new_country = (Position)malloc(sizeof(Country));
	if (new_country == NULL) {
		perror("Could not allocate memory!");
		return NULL;
	}
	strcpy(new_country->name, country_name);
	new_country->cities_tree = NULL;
	new_country->next_country = NULL;

	return new_country;
}

Pointer CreateNewCity(char* name, int population)
{
	Pointer new_city = NULL;
	new_city = (Pointer)malloc(sizeof(City));
	if (new_city == NULL) {
		perror("Could not allocate memory!");
		return NULL;
	}
	strcpy(new_city->name, name);
	new_city->population = population;
	new_city->Left = NULL;
	new_city->Right = NULL;

	return new_city;
}

int AddNewSorted(Position New, Position head)
{
	Position p = head;
	while (p->next_country != NULL && strcmp(New->name, p->next_country->name) > 0)
		p = p->next_country;
	New->next_country = p->next_country;
	p->next_country = New;

	return EXIT_SUCCESS;
}

Pointer AddNewCity(Pointer New, Pointer parent)
{
	if (parent == NULL) {
		return New;
	}
	else if (New->population == parent->population) {
		if (strcmp(New->name, parent->name) < 0) {
			New->Right = parent;
			New->Left = parent->Left;
			parent->Left = NULL;
			return New;
		}
	}
	else if (New->population < parent->population) {
		parent->Left = AddNewCity(New, parent->Left);
		return parent;
	}
	else if (New->population > parent->population) {
		parent->Right = AddNewCity(New, parent->Right);
		return parent;
	}
}

int PrintList(Position p)
{
	while (p != NULL) {
		printf("%s\n", p->name);
		PrintCities(p->cities_tree);
		p = p->next_country;
	}
	
	return EXIT_SUCCESS;
}

int PrintCities(Pointer p)
{
	if(p->Left != NULL) PrintCities(p->Left);
	printf("\t%s %d\n", p->name, p->population);
	if (p->Right != NULL)PrintCities(p->Right);

	return EXIT_SUCCESS;
}

int UserInput(Position head)
{
	char country[MAX_LEN] = { 0 };
	int population = 0;
	Position p = head;

	printf("\nEnter name of the country: ");
	scanf(" %s", country);
	printf("\nEnter population number: ");
	scanf("%d", &population);
	
	while (p != NULL && strcmp(country, p->name) != 0)
		p = p->next_country;
	if (p == NULL) {
		printf("Country is not in the list!");
		return EXIT_FAILURE;
	}

	printf("Cities in %s with more than %d people:\n", country, population);
	if(FindMorePopulated(p->cities_tree, population) == 0)
		printf("There is no cities with more than %d people in %s.", population, country);

	return EXIT_SUCCESS;
}
int FindMorePopulated(Pointer city, int population)
{
	int city_num = 0;
	if (city->population > population) {
		city_num++;
		printf("\t%s %d\n", city->name, city->population);
		if (city->Right != NULL) PrintCities(city->Right);
		city_num += FindMorePopulated(city->Left, population);
	}
	else return city_num;
}
