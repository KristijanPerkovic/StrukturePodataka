#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 1024
#define ERROR -1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Element* Position;
typedef struct _Element
{
	int coeff, exp;
	Position next;
}Element;


Position CreateElement(int coeff, int exp);

int InsertAfter(Position previous, Position to_insert);

int DeleteAfter(Position temp);

int InsertSorted(Position pol, Position nE);

int StringToList(char* buffer, Position pol);

int ReadFile(Position pol1, Position pol2);

int PrintPolynomial(Position first);

int Addition(Position pol1, Position pol2, Position result);

int Multiply(Position pol1, Position pol2, Position result);

int main()
{
	Element pol1 = { .coeff = 0 , .exp = 0, .next = NULL };
	Element pol2 = { .coeff = 0 , .exp = 0, .next = NULL };
	Element polAddition = { .coeff = 0 , .exp = 0, .next = NULL };
	Element polMultiply = { .coeff = 0 , .exp = 0, .next = NULL };

	ReadFile(&pol1, &pol2);

	printf("First polynomial: ");
	PrintPolynomial(pol1.next);
	printf("\nSecond polynomial: ");
	PrintPolynomial(pol2.next);

	Addition(pol1.next, pol2.next, &polAddition);
	printf("\nAddition of polynomials: ");
	PrintPolynomial(polAddition.next);

	Multiply(pol1.next, pol2.next, &polMultiply);
	printf("\nPolynomials multiplied: ");
	PrintPolynomial(polMultiply.next);

	return 0;
}

Position CreateElement(int coeff, int exp)
{
	Position nE = (Position)malloc(sizeof(Element));
	if (!nE)
	{
		perror("Error. Malloc failed in function create_element.\n");
		return NULL;
	}

	nE->coeff = coeff;
	nE->exp = exp;
	nE->next = NULL;

	return nE;
}

int InsertAfter(Position previous, Position to_insert)
{
	if (previous == NULL || to_insert == NULL) return ERROR;

	to_insert->next = previous->next;
	previous->next = to_insert;

	return EXIT_SUCCESS;
}

int DeleteAfter(Position temp)
{
	Position to_delete = temp->next;

	if (!to_delete) return ERROR;
	temp->next = to_delete->next;
	free(to_delete);

	return EXIT_SUCCESS;
}

int InsertSorted(Position pol, Position nE)
{
	Position temp = pol;

	while (temp->next && temp->next->exp < nE->exp) temp = temp->next;

	if (temp->next == NULL || temp->next->exp != nE->exp) InsertAfter(temp, nE);
	else
	{
		int resultCoefficient = nE->coeff + temp->next->coeff;
		if (resultCoefficient == 0) DeleteAfter(temp);
		else temp->next->coeff = resultCoefficient;
		free(nE);
	}

	return EXIT_SUCCESS;
}

int StringToList(char* buffer, Position pol)
{
	char* current_buffer = buffer;
	int numBytes, c, e;
	Position nE = NULL;

	while (strlen(current_buffer) > 0)
	{
		sscanf(current_buffer, "%d %d %n", &c, &e, &numBytes);
		nE = CreateElement(c, e);
		if (!nE) return ERROR;
		InsertSorted(pol, nE);
		current_buffer += numBytes;
	}

	return EXIT_SUCCESS;
}

int ReadFile(Position pol1, Position pol2)
{
	FILE* fp = NULL;

	fp = fopen("polynomials.txt", "r");

	if (!fp)
	{
		perror("Error in opening file!\n");
		return ERROR;
	}

	char buffer[MAX_SIZE];

	fgets(buffer, MAX_SIZE, fp);
	StringToList(buffer, pol1);

	fgets(buffer, MAX_SIZE, fp);
	StringToList(buffer, pol2);

	fclose(fp);

	return EXIT_SUCCESS;
}

int PrintPolynomial(Position first)
{
	Position temp = first;

	while (temp)
	{
		if (temp->exp == 0) printf("%d ", temp->coeff, temp->exp);
		else printf("%dx^%d ", temp->coeff, temp->exp);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int Addition(Position pol1, Position pol2, Position result)
{
	Position RemainingPol = NULL;
	Position nE = NULL;

	while (pol1 && pol2)
	{
		if (pol1->exp == pol2->exp)
		{
			nE = CreateElement(pol1->coeff + pol2->coeff, pol1->exp);
			InsertSorted(result, nE);
			pol1 = pol1->next;
			pol2 = pol2->next;
		}
		else if (pol1->exp < pol2->exp)
		{
			nE = CreateElement(pol2->coeff, pol2->exp);
			InsertSorted(result, nE);
			pol2 = pol2->next;
		}
		else
		{
			nE = CreateElement(pol1->coeff, pol1->exp);
			InsertSorted(result, nE);
			pol1 = pol1->next;
		}
	}

	RemainingPol = (pol1) ? pol1 : pol2;

	while (RemainingPol)
	{
		nE = CreateElement(RemainingPol->coeff, RemainingPol->exp);
		InsertSorted(result, nE);
		RemainingPol = RemainingPol->next;
	}

	return EXIT_SUCCESS;
}

int Multiply(Position pol1, Position pol2, Position result)
{
	Position firstElemPol2 = pol2;
	Position nE = NULL;

	while (pol1) 
	{
		while (pol2) 
		{
			nE = CreateElement(pol1->coeff * pol2->coeff, pol1->exp + pol2->exp);
			InsertSorted(result, nE);
			pol2 = pol2->next;
		}
		pol2 = firstElemPol2;
		pol1 = pol1->next;
	}
	
	return EXIT_SUCCESS;
}