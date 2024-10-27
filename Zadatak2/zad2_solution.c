#define _CRT_SECURE_NO_WARNINGS
#define ERROR -1;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person* position;
typedef struct _person 
{
	//data
	char fname[32], lname[32];
	int birth_year;

	//link
	position next;

}person;

position create_person(char* fname, char* lname, int birth_year); // create an element in the list
int prepend_list(position head, char* fname, char* lname, int birth_year); // insert the element at the beginning of the list
int append_list(position head, char* fname, char* lname, int birth_year); // insert the element at the end of the list
int insert_after(position previous, position to_insert); // used in prepend_list
int print_list(position first); // prints the elements of the list
position find_by_lname(position first, char* lname); // finds the person by their last name
position find_last(position head); // finds the last element of the list
position find_previous(position head, position wanted); // finds the previous element of the wanted element
int delete_person(position head, position to_delete); // deletes the to_delete element

int main()
{
	person head = { .fname = "", .lname = "", .birth_year = 0, .next = NULL };

	prepend_list(&head, "Mate", "Matic", 2004);
	append_list(&head, "Ivo", "Ivic", 2000);
	prepend_list(&head, "Jure", "Juric", 1996);
	prepend_list(&head, "Mara", "Maric", 2002);
	print_list(head.next);

	char* searched_last_name = "Juric";
	char* ftp_last_name = "Ivic";
	char* to_delete_last_name = "Maric";

	person* searched = find_by_lname(head.next, searched_last_name);
	if(searched) printf("Searched: %s %s %d\n", searched->fname, searched->lname, searched->birth_year);

	person* find_their_previous = find_by_lname(head.next, ftp_last_name);
	person* previous = find_previous(&head, find_their_previous);
	if (find_their_previous && previous && find_their_previous != head.next) printf("The previous element of %s is %s %s %d.\n", ftp_last_name, previous->fname, previous->lname, previous->birth_year);

	person* last = find_last(&head);
	if (last && last != &head) printf("The last element of the list is %s %s %d.\n", last->fname, last->lname, last->birth_year);

	person* to_delete = find_by_lname(head.next, to_delete_last_name);
	delete_person(&head, to_delete);

	append_list(&head, "Ana", "Anic", 2005);

	print_list(head.next);

	return 0;
}

position create_person(char* fname, char* lname, int birth_year)
{
	position new_person = NULL;

	new_person = (position*)malloc(sizeof(person)); // memory allocation for new_person
	if (!new_person)
	{
		printf("Error. Malloc failed in function create_person.\n");
		return NULL;
	}

	strcpy(new_person->fname, fname);
	strcpy(new_person->lname, lname);
	new_person->birth_year = birth_year;
	new_person->next = NULL;

	return new_person;
}

int prepend_list(position head, char* fname, char* lname, int birth_year)
{
	position new_person = create_person(fname, lname, birth_year);
	if (!new_person) return ERROR;
	
	insert_after(head, new_person); // head is always the previous element when inserting the element at the beginning of the list

	return EXIT_SUCCESS;
}

int append_list(position head, char* fname, char* lname, int birth_year)
{
	position new_person = NULL;
	position last_person = NULL;

	new_person = create_person(fname, lname, birth_year);
	if (new_person == NULL) return ERROR;

	if (head->next == NULL) head->next = new_person; // if the list is empty
	else // if it is not empty
	{
		last_person = find_last(head);
		last_person->next = new_person;
	}

	new_person->next = NULL;

	return EXIT_SUCCESS;
}

int insert_after(position previous, position to_insert)
{
	if (previous == NULL || to_insert == NULL) return ERROR;

	to_insert->next = previous->next;
	previous->next = to_insert; // it does not matter if the list is empty (only head exists), the code always works

	return EXIT_SUCCESS;
}

int print_list(position first)
{
	position temp = first;

	while (temp) // if the list is empty, that means that first (which is head->next) is NULL, so nothing will be printed out
	{
		printf("%s %s %d\n", temp->fname, temp->lname, temp->birth_year);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

position find_by_lname(position first, char* lname)
{
	position temp = first;

	while (temp)
	{
		if (strcmp(temp->lname, lname) == 0) return temp; // compares temp->lname and the searched last name
		temp = temp->next;
	}
	printf("The element %s does not exist in the list or the list is empty.\n", lname);

	return NULL;
}

position find_last(position head)
{
	position temp = head;

	while (temp->next) temp = temp->next; // go until the element links to NULL. When it does, we have found the last element

	if(temp == head) printf("Unable to find the last element because the list is empty.\n");
	
	return temp;
}

position find_previous(position head, position wanted)
{

	position temp = head;
	
	while (temp->next)
	{
		if (temp->next == wanted) return temp; // temp is the previous element of wanted (temp->next links to wanted)
		temp = temp->next;
	}

	if (head->next == wanted && head->next)
	{
		printf("The first element has no previous element.\n");
		return NULL;
	}

	if (wanted == head) printf("Unable to find the previous element of head.\n");

	return NULL;
}

int delete_person(position head, position to_delete)
{
	position temp = head;

	if (to_delete == NULL) return EXIT_FAILURE;
	
	if (head == to_delete)
	{
		printf("Unable to delete head.\n");
		return EXIT_FAILURE;
	}
	else
	{
		position previous = find_previous(head, to_delete);
		char last_name[32];
		strcpy(last_name, to_delete->lname);

		if (previous == NULL) head->next = to_delete->next; // if we are deleting the first element of the list
		else previous->next = to_delete->next; // if we are deleting other elements of the list

		free(to_delete); // frees the allocated memory for the deleted element
		printf("Successfully deleted %s from the list.\n", last_name);
	}
	return EXIT_SUCCESS;
}