#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _binTree* position;
typedef struct _binTree
{
	int el;
	position left, right;
}binTree;

position create_root(int element);

position insert_element(position root, int element);

int inorder(position root);

int preorder(position root);

int postorder(position root);

int height(position root);

int print_level(position root, int level);

int level_order(position root);

int search_element(position root, int element);

position min_element(position root);

position delete_element(position root, int element);

void menu(position root);

int main() 
{
	position root = NULL;
	int elements[] = { 6, 2, 1, 4, 3, 8, 5};
	int n = sizeof(elements) / sizeof(elements[0]);
	for (int i = 0; i < n; i++) root = insert_element(root, elements[i]);

	menu(root);

	return 0;
}

position create_root(int element)
{
	position new = (position)malloc(sizeof(binTree));
	if (!new)
	{
		printf("Memory allocation error in create_root!\n");
		return NULL;
	}

	new->el = element;
	new->left = new->right = NULL;

	return new;
}

position insert_element(position root, int element)
{
	if (!root) return create_root(element);

	if (element < root->el)
		root->left = insert_element(root->left, element);
	else if (element > root->el)
		root->right = insert_element(root->right, element);

	return root;
}

int inorder(position root)
{
	if (root)
	{
		inorder(root->left);
		printf("%d ", root->el);
		inorder(root->right);
	}

	return EXIT_SUCCESS;
}

int preorder(position root)
{
	if (root)
	{
		printf("%d ", root->el);
		preorder(root->left);
		preorder(root->right);
	}

	return EXIT_SUCCESS;
}

int postorder(position root)
{
	if (root)
	{
		postorder(root->left);
		postorder(root->right);
		printf("%d ", root->el);
	}

	return EXIT_SUCCESS;
}

int height(position root)
{
	if (!root) return 0;
	int left_height = height(root->left);
	int right_height = height(root->right);

	return (left_height > right_height ? left_height : right_height) + 1;
}

int print_level(position root, int level)
{
	if (!root) return EXIT_FAILURE;

	if (level == 1) printf("%d ", root->el);
	else if (level > 1)
	{
		print_level(root->left, level - 1);
		print_level(root->right, level - 1);
	}

	return EXIT_SUCCESS;
}

int level_order(position root)
{
	int h = height(root);

	for (int i = 1; i <= h; i++) print_level(root, i);

	return EXIT_SUCCESS;
}

int search_element(position root, int element)
{
	if (!root) return 0;
	if (root->el == element) return 1;
	if (element < root->el)	return search_element(root->left, element);
	return search_element(root->right, element);
}

position min_element(position root)
{
	position temp = root;
	while (temp->left)
		temp = temp->left;
	return temp;
}

position delete_element(position root, int element)
{
	if (!root)	return root;
	if (element < root->el)
		root->left = delete_element(root->left, element);
	else if (element > root->el)
		root->right = delete_element(root->right, element);
	else
	{
		if (!root->left)
		{
			position temp = root->right;
			free(root);
			return temp;
		}
		else if (!root->right)
		{
			position temp = root->left;
			free(root);
			return temp;
		}
		position temp = min_element(root->right);
		root->el = temp->el;
		root->right = delete_element(root->right, temp->el);
	}
	return root;
}

void menu(position root)
{
	int choice, element;
	printf("BINARY TREE MENU!!!\n\n");
	printf("1. Insert element\n");
	printf("2. Inorder\n");
	printf("3. Preorder\n");
	printf("4. Postorder\n");
	printf("5. Level order\n");
	printf("6. Search for element\n");
	printf("7. Delete element\n");
	printf("8. Exit menu\n");
	do 
	{
		printf("\nType your choice (1 - 8): ");	
		scanf("%d", &choice);

		switch (choice) 
		{
			case 1:
				printf("Type your number: ");
				scanf("%d", &element);
				root = insert_element(root, element);
				break;
			case 2:
				inorder(root);
				printf("\n");
				break;
			case 3:
				preorder(root);
				printf("\n");
				break;
			case 4:
				postorder(root);
				printf("\n");
				break;
			case 5:
				level_order(root);
				printf("\n");
				break;
			case 6:
				printf("Number to search: ");	
				scanf("%d", &element);
				if (search_element(root, element)) printf("The number %d exists in the binary tree.\n", element);
				else printf("The number %d does not exist in the binary tree.\n", element);
				break;
			case 7:
				printf("Number to delete: ");	
				scanf("%d", &element);
				root = delete_element(root, element);
				printf("Successfully deleted %d from the binary tree!\n", element);
				break;
			case 8:
				printf("Successfully exited the menu!\n");
				break;
			default:
				printf("Invalid choice! Please type a valid choice.\n");
		}
	} while (choice != 8);
}