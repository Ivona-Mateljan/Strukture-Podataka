#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX 128
#define MAX_LEN 1024
#define NUM_RAND 10

struct _Tree;
typedef struct _Tree* Child;
typedef struct _Tree* Element;

typedef struct _Tree {
	int El;
	Child Left;
	Child Right;
}Tree;

int OpenFile(char*, Element);
int ReadFromFile(char*, Element);
Element CreateNewElement(int);
Element Insert(Element, Element);
int Replace(Element);
int RandomNumberGenerator(Element);
int PrintinFile(Element, char*);
int LegitFunctionToPrintNewLine(char*);

int main()
{
	Tree root = { .El = 0, .Left = NULL, .Right = NULL };
	char file_name[] = "brojevi.txt";
	char* file = &file_name;
	char file_rand[] = "random.txt";
	char* rand = &file_rand;
	int i = 0;

	printf("Please choose one of the options:\n"
		"\t1.\tPredifined numbers from brojevi.txt document\n"
		"\t2.\tRandom numbers\n");
	scanf("%d", &i);

	switch (i)
	{
	case 1:
		if (OpenFile(file, &root) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		Replace(&root);
		LegitFunctionToPrintNewLine(file);
		PrintinFile(&root, file);
		break;
	case 2:
		RandomNumberGenerator(&root);
		PrintinFile(&root, rand);
		Replace(&root);
		LegitFunctionToPrintNewLine(rand);
		PrintinFile(&root, rand);
		break;
	default:
		printf("You didn't choose any of the options!");
		break;
	}
	if (OpenFile(file, &root) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int OpenFile(char* file_name, Element root)
{
	FILE* fp = NULL;

	fp = fopen(file_name, "r");

	if (fp == NULL) {
		printf("File not found!");
		return EXIT_FAILURE;
	}

	if (ReadFromFile(file_name, root) != EXIT_SUCCESS)
		return EXIT_FAILURE;

	fclose(fp);

	return EXIT_SUCCESS;
}

int ReadFromFile(char* file_name, Element root)
{
	FILE* fp = NULL;
	Element new_el = NULL;
	char buffer[MAX_LEN] = { 0 };
	char* p_buffer = &buffer;
	int num = 0;
	int num_bytes = 0;

	fp = fopen(file_name, "r");

	fgets(buffer, MAX_LEN, fp);

	if (strlen(p_buffer) == 0) {
		printf("File is empty!");
		return 1;
	}
	sscanf(p_buffer, "%d %n", &num, &num_bytes);
	root->El = num;
	p_buffer += num_bytes;

	while (strlen(p_buffer) != 0) {
		sscanf(p_buffer, "%d %n", &num, &num_bytes);
		new_el = CreateNewElement(num);
		if (new_el == NULL) return EXIT_FAILURE;
		root = Insert(root, new_el);
		p_buffer += num_bytes;
	}

	return EXIT_SUCCESS;
}

int RandomNumberGenerator(Element root)
{
	int i = 0;
	int num = 0;
	Element new_el = NULL;
	srand((unsigned)time(NULL));

	for (i = 0; i < NUM_RAND; i++) {
		num = rand() % (90 - 10 + 1) + 10;
		new_el = CreateNewElement(num);
		if (new_el == NULL) return EXIT_FAILURE;
		root = Insert(root, new_el);
	}

	return EXIT_SUCCESS;
}

Element CreateNewElement(int num) {
	Element new_el = (Element)malloc(sizeof(Tree));

	if (new_el == NULL) {
		printf("Couldn't allocate memory for new element!");
		return NULL;
	}

	new_el->El = num;
	new_el->Right = NULL;
	new_el->Left = NULL;

	return new_el;
}

Element Insert(Element parent, Element new_el)
{
	if (parent == NULL) return new_el;
	else if (parent->El <= new_el->El)
		parent->Left = Insert(parent->Left, new_el);
	else if (parent->El > new_el->El)
		parent->Right = Insert(parent->Right, new_el);
	return parent;
}

int Replace(Element p)
{
	if (p->Left == NULL && p->Right == NULL) {
		int temp = p->El;
		p->El = 0;
		return temp;
	}
	else if (p->Right == NULL){
		int temp = p->El;
		p->El = Replace(p->Left);
		return temp + p->El;
	}
	else if (p->Left == NULL) {
		int temp = p->El;
		p->El = Replace(p->Right);
		return temp + p->El;
	}
	else {
		int temp = p->El;
		p->El = Replace(p->Right) + Replace(p->Left); 
		return temp + p->El;
	}

	return p->El;
}

int PrintinFile(Element p, char* file)
{
	FILE* fp = NULL;

	fp = fopen(file, "a");
	if (fp == NULL) {
		perror("Couldn't open file!");
		return EXIT_FAILURE;
	}
	if (p->Left != NULL) PrintinFile(p->Left, file);
	fprintf(fp, "%d ", p->El);
	if (p->Right != NULL) PrintinFile(p->Right, file);

	//fprintf(fp, "%s", "\n");
	fclose(fp);

	return EXIT_SUCCESS;
}
int LegitFunctionToPrintNewLine(char* file)
{
	FILE* fp = NULL;
	fp = fopen(file, "a");

	if (fp == NULL) {
		perror("Couldn't open file!");
		return EXIT_FAILURE;
	}
	fprintf(fp, "%s", "\n");
	fclose(fp);

	return EXIT_SUCCESS;
}