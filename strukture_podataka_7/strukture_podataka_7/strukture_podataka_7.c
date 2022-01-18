#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 128

struct _RootTree;
typedef struct _RootTree* Tree;
typedef struct _RootTree* Position;
typedef struct _RootTree {
	char folder[MAX];
	Position Child;
	Position Sibiling;
}RootTree;

int TotallyNecessaryFunction(char*);
int PrintDirectory(Tree);
Tree NewFolder(Tree, char*);
Tree MakeNewFolder(Tree, char*);
Tree ChangeDirectory(Tree, char*);
Tree GoBack(Tree, Tree, Tree);

int main()
{
	RootTree Directory = { .folder = "students", .Child = NULL, .Sibiling = NULL };
	Tree Dir = &Directory;
	Tree CurrentDir = Dir;
	Tree CurrentDirCheck = NULL;
	char command[5] = { 0 };
	char name[MAX] = { 0 };
	char string[MAX] = { 0 };

	printf("Command prompt\n");

	while (strcmp(command, "exit") != 0) {
		printf("\nC:/%s>", CurrentDir->folder);
		scanf(" %s", command);
		switch (TotallyNecessaryFunction(command))
		{
		case 1:
			scanf(" %s", name);
			Tree SubDir = CurrentDir->Child;
			CurrentDir->Child = NewFolder(SubDir, name);
			break;

		case 2:
			scanf(" %s", name);
			CurrentDirCheck = ChangeDirectory(CurrentDir->Child, name);
			if (CurrentDirCheck != NULL) CurrentDir = CurrentDirCheck;
			break;

		case 3:
			CurrentDir = GoBack(Dir, Dir, CurrentDir);
			break;

		case 4:
			PrintDirectory(CurrentDir);
			break;

		case 5:
			printf("Exiting DOS...\n");
			break;

		default:
			printf("'%s' is not recognized as an internal or external command, "
				"operable program or batch file.\n", command);
			break;
		}
	}

	return 0;
	
}

int TotallyNecessaryFunction(char* string)
{
	if (strcmp(string, "md") == 0) return 1;
	else if (strcmp(string, "cd") == 0) return 2;
	else if (strcmp(string, "cd..") == 0) return 3;
	else if (strcmp(string, "dir") == 0) return 4;
	else if (strcmp(string, "exit") == 0) return 5;
	else return -1;
}

Tree NewFolder(Tree SubDir, char* folder_name)
{
	if (SubDir == NULL) 
		SubDir = MakeNewFolder(SubDir, folder_name);
	else 
		SubDir->Sibiling = NewFolder(SubDir->Sibiling, folder_name);
	return SubDir;
}
	
Tree MakeNewFolder(Tree NewFolder, char* name)
{
	NewFolder = (Tree*)malloc(sizeof(RootTree));
	strcpy(NewFolder->folder, name);
	NewFolder->Child = NULL;
	NewFolder->Sibiling = NULL;
	return NewFolder;
}

int PrintDirectory(Tree Dir)
{
	printf("%s", Dir->folder);
	printf("\n\t");
	if (Dir->Child != NULL) {
		Dir = Dir->Child;
		if (Dir->Child != NULL) printf("<DIR>");
		printf("\t");
		printf("%s\n", Dir);
	}
	while (Dir->Sibiling != NULL) {
		Dir = Dir->Sibiling;
		if (Dir->Child != NULL) printf("\t<DIR>\t");
		else printf("\t\t");
		printf("%s\n", Dir);
	}

	return 0;
}

Tree ChangeDirectory(Tree SubDir, char* NewDir)
{
	if (SubDir == NULL) {
		printf("The system cannot find the path specified.\n");
		return NULL;
	}
	else if (strcmp(SubDir->folder, NewDir) != 0) {
		SubDir = ChangeDirectory(SubDir->Sibiling, NewDir);
	}
	else return SubDir;
}

//DirTF - Directory To Find
Tree GoBack(Tree CheckDir, Tree ParentDir, Tree DirTF)
{
	if (CheckDir == DirTF) return ParentDir;
	if (CheckDir->Sibiling != NULL) { 
		ParentDir = GoBack(CheckDir->Sibiling, ParentDir, DirTF);
		if (ParentDir == NULL) {
			if (CheckDir->Child != NULL) {
				CheckDir = GoBack(CheckDir->Child, CheckDir, DirTF);
				if (CheckDir != NULL)
					return CheckDir;
				else return NULL;
			}
			else return CheckDir;
		}
		else return ParentDir;
	}
	if (CheckDir->Child != NULL) {
		CheckDir = GoBack(CheckDir->Child, CheckDir, DirTF);
		if (CheckDir != NULL)
			return CheckDir;
		else return NULL;
	}
	else return NULL;
}