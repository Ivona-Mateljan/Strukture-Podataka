#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 128
#define MAX_LEN 1024

struct _Tree;
typedef struct _Tree* Child;
typedef struct _Tree* Element;

struct _Queue;
typedef struct _Queue* Position;

typedef struct _Tree {
	int El;
	Child Left;
	Child Right;
}Tree;

typedef struct _Queue {
	int num;
	Position next;
}Queue;

int OpenFile(char*, Element);
int ReadFromFile(char*, Element);
Element CreateNewElement(int);
Element AddElementToTree(Element, Element);
int PrintLevelOrder(Element, Position);
int Push(Element, Position);
int Pop(Position);
Element FindElementInTree(int, Element);

int main() 
{	
	Tree root = { .El = 0, .Left = NULL, .Right = NULL };
	Queue head = { .num = 0, .next = NULL };
	Position q = &head;
	char file_name[] = "tree_elements.txt";
	char* file = &file_name;
	
	if (OpenFile(file, &root) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	
	PrintLevelOrder(&root, q);

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
	
	if(ReadFromFile(file_name, root) != EXIT_SUCCESS)
		return EXIT_FAILURE;

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

	while(strlen(p_buffer) != 0) {
		sscanf(p_buffer, "%d %n", &num, &num_bytes);
		new_el = CreateNewElement(num);
		if (new_el == NULL) return EXIT_FAILURE;
		root = AddElementToTree(root, new_el);
		p_buffer += num_bytes;
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

Element AddElementToTree(Element parent, Element new_el)
{
	if (parent == NULL) return new_el;
	else if (parent->El > new_el->El)
		parent->Left = AddElementToTree(parent->Left, new_el);
	else if (parent->El < new_el->El)
		parent->Right = AddElementToTree(parent->Right, new_el);
	else if (parent->El == new_el->El)
		free(new_el);
	return parent;
}

int PrintLevelOrder(Element root, Position head_q)
{
	Element p = root;
	Push(root, head_q);
	while (head_q->next != NULL) {
		p = FindElementInTree(head_q->next->num, root);
		if (p->Left != NULL) Push(p->Left, head_q);
		if (p->Right != NULL) Push(p->Right, head_q);
		Pop(head_q);
	}
	
	return EXIT_SUCCESS;
}

Element FindElementInTree(int num, Element p) {
	while (p->El != num) {
		if (p->El > num) p = p->Left;
		else if (p->El < num) p = p->Right;
	}
	return p;
}

int Push(Element el, Position p)
{	
	Position q = p;
	Position New = (Position)malloc(sizeof(Queue));

	if (New == NULL) {
		printf("Couldn't allocate memory!");
		return EXIT_FAILURE;
	}

	New->num = el->El;
	while (q->next != NULL)
		q = q->next;
	q->next = New;
	New->next = NULL;

	return EXIT_SUCCESS;
}

int Pop(Position q)
{
	Position toFree = q->next;
	printf("%d ", toFree->num);
	q->next = toFree->next;
	free(toFree);

	return EXIT_SUCCESS;
}