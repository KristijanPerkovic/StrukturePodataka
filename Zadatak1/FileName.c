#define _CRT_SECURE_NO_WARNINGS
#define ERROR_OPENING_FILE 1
#define BUFFER_SIZE 1024
#define MAX_SCORE 50
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char name[20], surname[20];
	float score;
}_stud;

int count_rows(char* filename);

int read_file(char* filename, int row_count, _stud* students);

void print_students(int row_count, _stud* students);


int main()
{

	int row_count = count_rows("students.txt");

	_stud* students = (_stud*)malloc(row_count * sizeof(_stud));

	read_file("students.txt", row_count, students);

	print_students(row_count, students);

	return 0;
}

int count_rows(char* filename)
{
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	int row_count = 0;

	fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("Error opening file\n");
		return ERROR_OPENING_FILE;
	}

	while (!feof(fp))
	{
		fgets(buffer, BUFFER_SIZE, fp);
		++row_count;
	}

	fclose(fp);

	return row_count;
}

int read_file(char* filename, int row_count, _stud* students)
{
	FILE* fp = NULL;

	fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("Error opening file\n");
		return ERROR_OPENING_FILE;
	}

	for (int i = 0; i < row_count; i++)
		fscanf(fp, "%s %s %f", students[i].name, students[i].surname, &students[i].score);

	fclose(fp);

	return EXIT_SUCCESS;
}

void print_students(int row_count, _stud* students)
{
	for (int i = 0; i < row_count; i++)
		printf("%s %s %.2f %.2f\n", students[i].name, students[i].surname, students[i].score, students[i].score / MAX_SCORE * 100);

	free(students);
}