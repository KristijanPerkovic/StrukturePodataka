#define _CRT_SECURE_NO_WARNINGS
#define ERROR -1;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _person* position;
typedef struct _person
{
	//data
	char fname[32], lname[32];
	int birth_year;

	//link
	position next;

}person;

void menu(position head); // menu
position create_person(char* fname, char* lname, int birth_year); // create an element in the list
int prepend_list(position head, char* fname, char* lname, int birth_year); // insert the element at the beginning of the list
int append_list(position head, char* fname, char* lname, int birth_year); // insert the element at the end of the list
int insert_after(position previous, position to_insert); // used in prepend_list
int insert_before(position head, position to_be_inserted_before, char* fname, char* lname, int birth_year); // inserts an element before another element
int print_list(position first); // prints the elements of the list
position find_by_lname(position first, char* lname); // finds the person by their last name
position find_last(position head); // finds the last element of the list
position find_previous(position head, position wanted); // finds the previous element of the wanted element
int delete_person(position head, position to_delete); // deletes the to_delete element
void sort(position head); // sort list
int create_list_file(position first, char* filename); // creates a file and writes the elements of the list in it
int read_list_file(position head, char* filename); // reads a file and creates a list of elements from it

int main()
{
	person head = { .fname = "", .lname = "", .birth_year = 0, .next = NULL };

	menu(&head);

	return 0;
}

void menu(position head)
{
	char fname[32], lname[32], lname2[32];
	int birth_year, choice = 0;

	printf("MENU - LIST FUNCTIONS");

	while (choice != 13)
	{
		printf("\n\n\n1. Add student to the beginning of the list\n");
		printf("2. Add student to the end of the list\n");
		printf("3. Insert student after another student\n");
		printf("4. Insert student before another student\n");
		printf("5. Print the list of students\n");
		printf("6. Search information about student\n");
		printf("7. Find the last student\n");
		printf("8. Find the previous student of another student\n");
		printf("9. Delete a student from the list\n");
		printf("10. Sort the list by last names\n");
		printf("11. Create a .txt file from the list of students\n");
		printf("12. Add new students to the list from a .txt file\n");
		printf("13. Exit menu\n\n\n");

		printf("Choose action: ");
		scanf("%d", &choice);
		printf("\n");

		switch (choice)
		{
		case 1:
			printf("Write the new student's first name, last name and birth year ([fname] [lname] [birth year]): ");
			scanf("%s %s %d", fname, lname, &birth_year);
			prepend_list(head, fname, lname, birth_year);
			printf("Successfully added %s %s %d to the beginning of the list!\n", fname, lname, birth_year);
			break;
		case 2:
			printf("Write the new student's first name, last name and birth year ([fname] [lname] [birth year]): ");
			scanf("%s %s %d", fname, lname, &birth_year);
			append_list(head, fname, lname, birth_year);
			printf("Successfully added %s %s %d to the end of the list!\n", fname, lname, birth_year);
			break;
		case 3:
			if (head->next)
			{
				printf("Write the new student's first name, last name and birth year ([fname] [lname] [birth year]): ");
				scanf("%s %s %d", fname, lname, &birth_year);
				person* new_person = create_person(fname, lname, birth_year);
				printf("Which student would you like to insert the new student after (write only last name): ");
				scanf("%s", lname2);
				person* to_be_inserted_after = find_by_lname(head->next, lname2);
				if (insert_after(to_be_inserted_after, new_person) == EXIT_SUCCESS)
					printf("Successfully inserted %s %s %d after %s!\n", fname, lname, birth_year, lname2);
				else printf("Unable to insert %s %s %d", fname, lname, birth_year);
			}
			else printf("Unable to insert student because the list is empty!\n");
			break;
		case 4: 
			if (head->next)
			{
				printf("Write the new student's first name, last name and birth year ([fname] [lname] [birth year]): ");
				scanf("%s %s %d", fname, lname, &birth_year);
				printf("Which student would you like to insert the new student before (write only last name): ");
				scanf("%s", lname2);
				person* to_be_inserted_before = find_by_lname(head->next, lname2);
				if (insert_before(head, to_be_inserted_before, fname, lname, birth_year) == EXIT_SUCCESS)
					printf("Successfully inserted %s %s %d before %s!\n", fname, lname, birth_year, lname2);
				else printf("Unable to insert %s %s %d", fname, lname, birth_year);
			}
			else printf("Unable to insert student because the list is empty!\n");
			break;
		case 5:
			if (head->next) print_list(head->next);
			else printf("List has no students!\n");
			break;
		case 6: 
			if (head->next)
			{
				printf("Which student would you like to search information about (write only last name): ");
				scanf("%s", lname);
				person* searched = find_by_lname(head->next, lname);
				if (searched) printf("Searched: %s %s %d\n", searched->fname, searched->lname, searched->birth_year);
			}
			else printf("Unable to search the student because the list is empty!\n");
			break;
		case 7:
			person* last = find_last(head);
			if (last && last != head) printf("The last element of the list is %s %s %d.\n", last->fname, last->lname, last->birth_year);
			break;
		case 8: 
			if (head->next)
			{
				printf("Which student would you like to find the previous student for (write only last name): ");
				scanf("%s", lname);
				person* find_their_previous = find_by_lname(head->next, lname);
				person* previous = find_previous(head, find_their_previous);
				if (find_their_previous && previous && find_their_previous != head->next)
					printf("The previous element of %s is %s %s %d.\n", lname, previous->fname, previous->lname, previous->birth_year);
				else if (find_their_previous == head->next && head->next) printf("The first element has no previous element.\n");
			}
			else printf("Unable to find previous student because the list is empty!\n");
			break;
		case 9: 
			if (head->next)
			{
				printf("Which student would you like to delete from the list (write only last name): ");
				scanf("%s", lname);
				person* to_delete = find_by_lname(head->next, lname);
				delete_person(head, to_delete);
			}
			else printf("Unable to delete student because the list is empty!\n");
			break;
		case 10: 
			sort(head);

			break;
		case 11: 
			if (head->next)
			{
				printf("What would you like the name of the file to be (include .txt at the end): ");
				scanf("%s", fname);
				create_list_file(head->next, fname);
			}
			else printf("Unable to create .txt file because the list is empty!\n");
			break;
		case 12:
			printf("What is the name of the file you want to create a list from (include .txt at the end): ");
			scanf("%s", fname);
			read_list_file(head, fname);
			break;
		case 13: 
			printf("Successfully exited menu!\n");
			break;
		default: printf("Invalid choice! Please choose valid choice 1 - 13!\n");
		}
	}
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

int insert_before(position head, position to_be_inserted_before, char* fname, char* lname, int birth_year)
{
	position to_be_inserted_after = find_previous(head, to_be_inserted_before);
	position to_insert = create_person(fname, lname, birth_year);
	if (to_insert == NULL || to_be_inserted_before == NULL) return ERROR;
	insert_after(to_be_inserted_after, to_insert);
	return EXIT_SUCCESS;
}

int print_list(position first)
{
	position temp = first;

	printf("Students: \n");
	int i = 1;
	while (temp) // if the list is empty, that means that first (which is head->next) is NULL, so nothing will be printed out
	{
		printf("%d. %s %s %d\n", i, temp->fname, temp->lname, temp->birth_year);
		temp = temp->next;
		++i;
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

	if (temp == head) printf("Unable to find the last element because the list is empty!\n");

	return temp;
}

position find_previous(position head, position wanted)
{

	position temp = head;

	while (temp->next)
	{
		if (temp->next == wanted)
			return temp; // temp is the previous element of wanted (temp->next links to wanted)
		temp = temp->next;
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

void sort(position head)
{
	position current = head;
	if (!current->next || !current->next->next)
	{
		printf("Unable to sort the list because it has less than 2 elements.\n");
		return;
	}

	bool swapped = true;

	while (swapped)
	{
		swapped = false;
		current = head;
		while (current->next->next)
		{
			if (strcmp(current->next->lname, current->next->next->lname) > 0)
			{
				position first = current->next;
				position second = first->next;
				current->next = second;
				first->next = second->next;
				second->next = first;
				swapped = true;
			}
			current = current->next;
		}
	}

	printf("Successfully sorted the list of students!\n");
}

int create_list_file(position first, char* filename)
{
	position temp = first;

	FILE* people_file = NULL;
	people_file = fopen(filename, "w");
	if (!people_file)
	{
		printf("Error in opening file %s!\n", filename);
		return ERROR;
	}

	while (temp)
	{
		fprintf(people_file, "%s %s %d", temp->fname, temp->lname, temp->birth_year);
		if(temp->next) fprintf(people_file, "\n");
		temp = temp->next;
	}

	fclose(people_file);

	printf("Successfully created file %s!\n", filename);

	return EXIT_SUCCESS;
}

int read_list_file(position head, char* filename)
{
	FILE* people_file = NULL;
	people_file = fopen(filename, "r");
	if (!people_file)
	{
		printf("Error in opening file %s!\n", filename);
		return ERROR;
	}

	char fname[32], lname[32];
	int birth_year;

	while (fscanf(people_file, "%s %s %d", fname, lname, &birth_year) == 3)
		append_list(head, fname, lname, birth_year);

	fclose(people_file);

	printf("Successfully appended elements to the list from file %s!\n", filename);

	return EXIT_SUCCESS;
}