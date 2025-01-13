#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _city* tree_position;
typedef struct _city
{
	int population;
	char name[32];
	tree_position left, right;
}City;

typedef struct _country* list_position;
typedef struct _country
{
	char name[32];
	list_position next;
	tree_position root;
}Country;

int inorder(tree_position root);
int print_countries_cities(list_position first);
tree_position create_city(char* name, int population);
tree_position insert_city_sorted_by_population(tree_position root, char* name, int population);
tree_position insert_city_sorted_by_name(tree_position root, char* name, int population);
int insert_country_sorted(list_position head, char* name, char* filename);
int read_country_file(list_position head);
int find_cities(tree_position root, int population);
int request(list_position head);

int main()
{
	Country head = { .root = NULL, .name = "", .next = NULL };
	read_country_file(&head);
	print_countries_cities(head.next);
	request(&head);

	return 0;
}

int inorder(tree_position root)
{
	if (root)
	{
		inorder(root->left);
		printf("%s: %d\n", root->name, root->population);
		inorder(root->right);
	}

	return EXIT_SUCCESS;
}

int print_countries_cities(list_position first)
{
	list_position temp = first;

	while (temp)
	{
		printf("%s\n", temp->name);
		inorder(temp->root);
		printf("\n");
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

tree_position create_city(char *name, int population)
{
	tree_position new_city = malloc(sizeof(City));
	if (!new_city)
	{
		printf("Memory allocation error in create_city!\n");
		return NULL;
	}

	strcpy(new_city->name, name);
	new_city->population = population;
	new_city->left = new_city->right = NULL;

	return new_city;
}

tree_position insert_city_sorted_by_population(tree_position root, char* name, int population)
{
	if (!root) return create_city(name, population);

	if (population < root->population)
		root->left = insert_city_sorted_by_population(root->left, name, population);
	else if (population > root->population)
		root->right = insert_city_sorted_by_population(root->right, name, population);

	return root;
}


tree_position insert_city_sorted_by_name(tree_position root, char* name, int population)
{
	if (!root) return create_city(name, population);

	if (strcmp(name, root->name) < 0)
		root->left = insert_city_sorted_by_name(root->left, name, population);
	else if (strcmp(name, root->name) > 0)
		root->right = insert_city_sorted_by_name(root->right, name, population);

	return root;
}

int insert_country_sorted(list_position head, char* name, char* filename)
{
	list_position temp = head;
	list_position new_country = malloc(sizeof(Country));

	char city_name[32];
	int population;

	if (!new_country)
	{
		printf("Error in allocating memory in insert_country_sorted!\n");
		return EXIT_FAILURE;
	}

	new_country->root = NULL;
	new_country->next = NULL;
	strcpy(new_country->name, name);

	FILE* fp = fopen(filename, "r");

	if (!fp)
	{
		printf("Error in opening file!\n");
		free(new_country);
		return EXIT_FAILURE;
	}

	while (fscanf(fp, "%s %d", city_name, &population) == 2)
	{
		//new_country->root = insert_city_sorted_by_population(new_country->root, city_name, population);
		new_country->root = insert_city_sorted_by_name(new_country->root, city_name, population);
		if(!new_country->root)
		{
			printf("Error in adding city to country!\n");
			fclose(fp);
			free(new_country->root);
			free(new_country);
			return EXIT_FAILURE;
		}
	}

	fclose(fp);

	while (temp->next && strcmp(temp->next->name, new_country->name) < 0)
		temp = temp->next;
	
	new_country->next = temp->next;
	temp->next = new_country;

	return EXIT_SUCCESS;
}

int read_country_file(list_position head)
{
	FILE* fp = fopen("drzave.txt", "r");
	char name[32], filename[32];
	if (!fp)
	{
		printf("Error in opening file!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(fp, "%s %s", name, filename) == 2) 
	{
		if (insert_country_sorted(head, name, filename)) 
		{
			printf("Error in adding country to linked list!\n");
			fclose(fp);
			return EXIT_FAILURE;
		}
	}

	fclose(fp);

	return EXIT_SUCCESS;
}

int find_cities(tree_position root, int population)
{
	if (root)
	{
		find_cities(root->left, population);
		if (root->population > population) printf("%s: %d\n", root->name, root->population);	
		find_cities(root->right, population);
	}

	return EXIT_SUCCESS;
}

int request(list_position head)
{
	list_position temp = head;

	char country[32];
	int population;
	printf("Country to search: ");
	scanf("%s", country);
	printf("Population threshold: ");
	scanf("%d", &population);

	while (temp && strcmp(country, temp->name) != 0) temp = temp->next;

	if (temp)
	{
		printf("%s's cities with the population larger than %d:\n", temp->name, population);
		find_cities(temp->root, population);

		return EXIT_SUCCESS;
	}

	printf("Country not found.\n");
	return EXIT_FAILURE;
}