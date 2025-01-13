#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _city* list_position;
typedef struct _city 
{
    int population;
    char name[32];
    list_position next;
} City;

typedef struct _country* tree_position;
typedef struct _country 
{
    char name[32];
    tree_position left, right;
    list_position head;
} Country;

int inorder(tree_position root);
list_position create_city(char* name, int population);
list_position insert_city_sorted_by_population(list_position head, char* name, int population);
list_position insert_city_sorted_by_name(list_position head, char* name, int population);
tree_position insert_country_sorted(tree_position root, char* name, char* filename);
int read_country_file(tree_position* root);
int find_cities(list_position head, int population);
int request(tree_position root);

int main() 
{
    tree_position root = NULL;
    read_country_file(&root);
    inorder(root);
    request(root);

    return 0;
}

int inorder(tree_position root) 
{
    if (root) 
    {
        inorder(root->left);
        printf("%s\n", root->name);
        list_position city = root->head;
        while (city) 
        {
            printf("%s: %d\n", city->name, city->population);
            city = city->next;
        }
        printf("\n");
        inorder(root->right);
    }

    return EXIT_SUCCESS;
}

list_position create_city(char* name, int population) 
{
    list_position new_city = malloc(sizeof(City));
    if (!new_city) 
    {
        printf("Memory allocation error in create_city!\n");
        return NULL;
    }

    strcpy(new_city->name, name);
    new_city->population = population;
    new_city->next = NULL;

    return new_city;
}

list_position insert_city_sorted_by_population(list_position head, char* name, int population) 
{
    list_position new_city = create_city(name, population);
    if (!new_city) return head;

    if (!head || head->population > population) 
    {
        new_city->next = head;
        return new_city;
    }

    list_position temp = head;
    while (temp->next && temp->next->population <= population) temp = temp->next;
    
    new_city->next = temp->next;
    temp->next = new_city;

    return head;
}

list_position insert_city_sorted_by_name(list_position head, char* name, int population) 
{
    list_position new_city = create_city(name, population);
    if (!new_city) return head;

    if (!head || strcmp(head->name, name) > 0) 
    {
        new_city->next = head;
        return new_city;
    }

    list_position temp = head;
    while (temp->next && strcmp(temp->next->name, name) < 0) temp = temp->next;
    
    new_city->next = temp->next;
    temp->next = new_city;

    return head;
}

tree_position insert_country_sorted(tree_position root, char* name, char* filename) 
{
    if (!root) 
    {
        tree_position new_country = malloc(sizeof(Country));
        if (!new_country) 
        {
            printf("Error in allocating memory for new country!\n");
            return NULL;
        }

        strcpy(new_country->name, name);
        new_country->left = new_country->right = NULL;
        new_country->head = NULL;

        FILE* fp = fopen(filename, "r");
        if (!fp) 
        {
            printf("Error in opening file %s!\n", filename);
            free(new_country);
            return NULL;
        }

        char city_name[32];
        int population;
        while (fscanf(fp, "%s %d", city_name, &population) == 2)
        {
            new_country->head = insert_city_sorted_by_population(new_country->head, city_name, population);
            //new_country->head = insert_city_sorted_by_name(new_country->head, city_name, population);
        }
        
        fclose(fp);

        return new_country;
    }

    if (strcmp(name, root->name) < 0) root->left = insert_country_sorted(root->left, name, filename);
    else if (strcmp(name, root->name) > 0) root->right = insert_country_sorted(root->right, name, filename);
    
    return root;
}

int read_country_file(tree_position *root) 
{
    FILE* fp = fopen("drzave.txt", "r");
    char name[32], filename[32];
    if (!fp) 
    {
        printf("Error in opening file drzave.txt!\n");
        return EXIT_FAILURE;
    }

    while (fscanf(fp, "%s %s", name, filename) == 2) *root = insert_country_sorted(*root, name, filename);
    
    fclose(fp);

    return EXIT_SUCCESS;
}

int find_cities(list_position head, int population) 
{
    while (head) 
    {
        if (head->population > population) printf("%s: %d\n", head->name, head->population);
        head = head->next;
    }

    return EXIT_SUCCESS;
}

int request(tree_position root) 
{
    char country[32];
    int population;

    printf("Country to search: ");
    scanf("%s", country);
    printf("Population threshold: ");
    scanf("%d", &population);

    while (root) 
    {
        if (strcmp(country, root->name) == 0) 
        {
            printf("%s's cities with population larger than %d:\n", root->name, population);
            find_cities(root->head, population);
            return EXIT_SUCCESS;
        }

        else if (strcmp(country, root->name) < 0) root = root->left;

        else root = root->right;    
    }

    printf("Country not found.\n");
    return EXIT_FAILURE;
}
