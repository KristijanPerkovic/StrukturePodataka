#define _CRT_SECURE_NO_WARNINGS
#define ERROR -1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _article* ArticlePosition;
typedef struct _article 
{
    char name[50];
    int quantity;
    double price;
    ArticlePosition next;
}Article;

typedef struct 
{
    int year, month, day;
}Date;

typedef struct _receipt* ReceiptPosition;
typedef struct _receipt 
{
    Date receipt_date;
    ArticlePosition head_A;
    ReceiptPosition next;
}Receipt;

int ReadFromMainReceiptFile(ReceiptPosition head_R);
int AddReceiptSorted(ReceiptPosition head_R, char file_name[]);
int AddArticleSorted(ArticlePosition head_A, char name[], int quantity, double price);
int Request(ReceiptPosition head_R);

int main()
{

    Receipt head_I = { .head_A = NULL, .next = NULL };

    if (ReadFromMainReceiptFile(&head_I))
    {
        printf("The program failed.\n");
        return ERROR;
    }

    if (Request(&head_I))
    {
        printf("The request failed.\n");
        return ERROR;
    }

	return EXIT_SUCCESS;
}

int ReadFromMainReceiptFile(ReceiptPosition head_R) 
{
    char file_name[50];

    FILE* fp = NULL;
    fp = fopen("racuni.txt", "r");

    if (fp == NULL) 
    {
        printf("Error in opening file!\n");
        return ERROR;
    }

    while (fscanf(fp, "%s", file_name) == 1) 
    {
        if (AddReceiptSorted(head_R, file_name))
        {
            printf("Error in adding receipt to linked list!\n");
            return ERROR;
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int AddReceiptSorted(ReceiptPosition head_R, char file_name[]) 
{
    char article_name[50];
    int quantity;
    double price;

    ReceiptPosition temp = head_R;

    ReceiptPosition new_receipt = (ReceiptPosition)malloc(sizeof(Receipt));

    if (new_receipt == NULL) 
    {
        printf("Memory allocation error!\n");
        return ERROR;
    }

    new_receipt->head_A = (ArticlePosition)malloc(sizeof(Article));
    if (new_receipt->head_A == NULL) 
    {
        printf("Memory allocation error!\n");
        return ERROR;
    }

    new_receipt->head_A->quantity = 0;
    new_receipt->head_A->price = 0;
    new_receipt->head_A->next = NULL;

    new_receipt->next = NULL;

    new_receipt->receipt_date.year = 0;
    new_receipt->receipt_date.month = 0;
    new_receipt->receipt_date.day = 0;

    FILE* fp = NULL;
    fp = fopen(file_name, "r");

    if (fp == NULL) 
    {
        printf("Error in opening file!\n");
        return ERROR;
    }

    if (fscanf(fp, "%d-%d-%d\n", &new_receipt->receipt_date.year, &new_receipt->receipt_date.month, &new_receipt->receipt_date.day) != 3)
    {
        printf("Error in reading receipt date!\n");
        return ERROR;
    }


    while (fscanf(fp, "%s %d %lf\n", article_name, &quantity, &price) == 3) 
    {
        if (AddArticleSorted(new_receipt->head_A, article_name, quantity, price)) 
        {
            printf("Error in adding article to receipt!\n");
            return ERROR;
        }
    }

    while (temp->next &&
        (temp->next->receipt_date.year < new_receipt->receipt_date.year ||
            (temp->next->receipt_date.year == new_receipt->receipt_date.year && temp->next->receipt_date.month < new_receipt->receipt_date.month) ||
            (temp->next->receipt_date.year == new_receipt->receipt_date.year && temp->next->receipt_date.month == new_receipt->receipt_date.month && temp->next->receipt_date.day < new_receipt->receipt_date.day)))
    {
        temp = temp->next;
    }

    new_receipt->next = temp->next;
    temp->next = new_receipt;

    fclose(fp);

    return EXIT_SUCCESS;
}

int AddArticleSorted(ArticlePosition head_A, char name[], int quantity, double price) 
{
    ArticlePosition temp = head_A;
    ArticlePosition new_article = (ArticlePosition)malloc(sizeof(Article));

    if (new_article == NULL) 
    {
        printf("Memory allocation error!\n");
        return ERROR;
    }

    strcpy(new_article->name, name);
    new_article->quantity = quantity;
    new_article->price = price;
    new_article->next = NULL;

    if (head_A->next) 
    {
        while (temp->next && strcmp(new_article->name, temp->next->name) > 0) 
            temp = temp->next;     
    }

    new_article->next = temp->next;
    temp->next = new_article;

    return EXIT_SUCCESS;
}

int Request(ReceiptPosition head_I) 
{
    char name[50];
    Date start;
    Date end;

    int quantity = 0;
    double price = 0.0;
    int flag = 1;

    ReceiptPosition temp = head_I->next;

    printf("\nType the name of the article: ");
    scanf("%s", name);

    printf("Type the starting date in YYYY-MM-DD format: ");
    scanf("%d-%d-%d", &start.year, &start.month, &start.day);

    printf("Type the ending date in YYYY-MM-DD format: ");
    scanf("%d-%d-%d", &end.year, &end.month, &end.day);

    while (temp) 
    {
        ArticlePosition temp_A = temp->head_A->next;

        if (temp->receipt_date.year > end.year || (temp->receipt_date.year == end.year && (temp->receipt_date.month > end.month || (temp->receipt_date.month == end.month && temp->receipt_date.day > end.day))))
            break;
        
        if (temp->receipt_date.year > start.year || (temp->receipt_date.year == start.year && (temp->receipt_date.month > start.month || (temp->receipt_date.month == start.month && temp->receipt_date.day >= start.day))))
        {
            while (temp_A) 
            {
                if (strcmp(temp_A->name, name) == 0) 
                {
                    flag = 0;
                    quantity += temp_A->quantity;
                    price += (temp_A->price * temp_A->quantity);
                }

                temp_A = temp_A->next;
            }
        }

        temp = temp->next;
    }

    if (flag) 
    {
        printf("The article does not exist in the receipts!\n");
        return ERROR;
    }

    printf("Bought article quantity: %d\n", quantity);
    printf("Total cost of bought article quantity: %.2lf\n", price);

    return EXIT_SUCCESS;
}