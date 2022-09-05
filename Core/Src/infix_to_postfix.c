#include "calculator.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack *create_stack(unsigned capacity)
{
    struct stack *stack = (struct stack *)malloc(sizeof(struct stack));

    stack->top = -1;
    stack->array = (double *)malloc(sizeof(double) * capacity);

    return stack;
}

void push(struct stack *stack, double ch)
{
    stack->array[++stack->top] = ch;
}

double pop(struct stack *stack)
{
    if (stack->top != -1)
        return stack->array[stack->top--];
}

int priority(char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    }
    return -1;
}

char *infix_to_postfix(char *exp)
{
    size_t i, k;
    struct stack *stack = create_stack(strlen(exp));
    char *postfix = (char *)malloc(sizeof(char));

    /* Looping over the infix string */
    for (i = 0, k = 0; exp[i]; i++)
    {
        if (isalnum((unsigned char)exp[i]) || exp[i] == '.')
        {
            postfix = realloc(postfix, sizeof(char) * (k + 1));
            postfix[k++] = exp[i];
        }
        else if (i != 0)
        {
            postfix = realloc(postfix, sizeof(char) * (k + 1));
            postfix[k++] = ' ';
        }

        if (!isalnum((unsigned char)exp[i]) && exp[i] == '(' && exp[i] != '.')
        {
            push(stack, exp[i]);
        }
        else if (!isalnum((unsigned char)exp[i]) && exp[i] == ')' && exp[i] != '.')
        {
            while (stack->array[stack->top] != '(')
            {
                postfix = realloc(postfix, sizeof(char) * (k + 1));
                postfix[k++] = pop(stack);
            }
            /* Remove ( */
            pop(stack);
        }
        else if (!isalnum((unsigned char)exp[i]) && exp[i] != '.')
        {
            if (priority(stack->array[stack->top]) >= priority(exp[i]))
            {
                postfix = realloc(postfix, sizeof(char) * (k + 2));
                postfix[k++] = pop(stack);
                postfix[k++] = ' ';
            }
            push(stack, exp[i]);
        }
    }

    while (stack->top != -1)
    {
        postfix = realloc(postfix, sizeof(char) * (k + 2));
        postfix[k++] = ' ';
        postfix[k++] = pop(stack);
    }

    free(stack);

    postfix = realloc(postfix, sizeof(char) * (k + 1));
    postfix[k] = '\0';
    printf("%s\n", postfix);
    printf("\n");

    return postfix;
}