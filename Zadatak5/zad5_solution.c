#define _CRT_SECURE_NO_WARNINGS
#define ERROR -1
#define MAX_SIZE 1024
#include <stdio.h>
#include <stdlib.h>

typedef struct _stack* position;

typedef struct _stack
{
    double number;
    position next;
} stack;

int push(position head, double number);
int pop(position head, double* result);
double read_file(position head);

int main()
{
    stack head = { .next = NULL };
    double result = read_file(&head);

    if (result != ERROR)
        printf("Postfix expression calculated: %.2lf\n", result);

    return 0;
}

int push(position head, double number)
{
    position nE = NULL;

    nE = (position)malloc(sizeof(stack));
    if (!nE)
    {
        printf("Memory allocation error!\n");
        return ERROR;
    }

    nE->number = number;
    nE->next = head->next;
    head->next = nE;

    return EXIT_SUCCESS;
}

int pop(position head, double* result)
{
    if (!head->next)
    {
        printf("Stack is already empty!\n");
        return ERROR;
    }

    position first = head->next;
    *result = first->number;
    head->next = first->next;
    free(first);

    return EXIT_SUCCESS;
}

double read_file(position head)
{
    FILE* fp = NULL;
    double result;

    fp = fopen("postfix.txt", "r");

    if (!fp)
    {
        perror("Error in opening file!\n");
        return ERROR;
    }

    char buffer[MAX_SIZE];

    while (fscanf(fp, "%s", buffer) != EOF)
    {
        if (buffer[0] >= '0' && buffer[0] <= '9') {
            double value = atof(buffer);
            if (push(head, value) == ERROR)
            {
                printf("Adding stack element error.\n");
                fclose(fp);
                return ERROR;
            }
        }
        else
        {
            double operand1, operand2;
            if (pop(head, &operand1) == ERROR || pop(head, &operand2) == ERROR)
            {
                printf("Removing stack element error.\n");
                fclose(fp);
                return ERROR;
            }

            switch (buffer[0])
            {
            case '+':
                if (push(head, operand1 + operand2) == ERROR)
                {
                    printf("Error adding result back on stack.\n");
                    fclose(fp);
                    return ERROR;
                }
                break;
            case '-':
                if (push(head, operand2 - operand1) == ERROR)
                {
                    printf("Error adding result back on stack.\n");
                    fclose(fp);
                    return ERROR;
                }
                break;
            case '*':
                if (push(head, operand1 * operand2) == ERROR)
                {
                    printf("Error adding result back on stack.\n");
                    fclose(fp);
                    return ERROR;
                }
                break;
            case '/':
                if (operand1 == 0)
                {
                    printf("Division by zero isn't allowed!\n");
                    fclose(fp);
                    return ERROR;
                }
                if (push(head, operand2 / operand1) == ERROR)
                {
                    printf("Error adding result back on stack.\n");
                    fclose(fp);
                    return ERROR;
                }
                break;
            default:
                printf("Unknown operator: %s\n", buffer);
                fclose(fp);
                return ERROR;
            }
        }
    }

    if (pop(head, &result) == ERROR)
    {
        printf("Invalid postfix term.\n");
        fclose(fp);
        return ERROR;
    }

    if (head->next != NULL)
    {
        printf("Invalid postfix term. Stack is not empty!\n");
        fclose(fp);
        return ERROR;
    }

    fclose(fp);

    return result;
}
