#include "calculator.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

volatile size_t invalid = 0;

double calculate(char *exp)
{
    volatile size_t i, count = 0;
    struct stack *stack = create_stack(strlen(exp));
    char *tmp;
    volatile double val1, val2;

    tmp = (char *)malloc(strlen(exp) * sizeof(char));

    for (i = 0; exp[i]; i++)
    {
        if (isdigit((unsigned char)exp[i]) || exp[i] == '.')
        {
            tmp[count++] = exp[i];
        }
        else if (exp[i] == ' ' && isdigit((unsigned char)exp[i - 1]))
        {
            tmp[count] = '\0';
            push(stack, atof(tmp));
            count = 0;
        }
        if (!isdigit((unsigned char)exp[i]) && exp[i] != ' ' && exp[i] != '.')
        {
            val1 = pop(stack);
            val2 = pop(stack);
            switch (exp[i])
            {
            case '+':
                push(stack, val2 + val1);
                break;
            case '-':
                push(stack, val2 - val1);
                break;
            case '*':
                push(stack, val2 * val1);
                break;
            case '/':
                if (val1 == 0)
                {
                    invalid = 1;
                }
                else
                {
                    push(stack, val2 / val1);
                }
                break;
            }
        }
    }
    return pop(stack);
}