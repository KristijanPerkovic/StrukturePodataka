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
	printf("lol");

	return 0;
}

