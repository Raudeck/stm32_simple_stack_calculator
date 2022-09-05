#ifndef CALCULATOR_H_
#define CALCULATOR_H_

struct stack {
    double *array;
    int top;
};

extern struct stack * create_stack(unsigned capacity);
extern void push(struct stack *stack, double ch);
extern double pop(struct stack *stack);
extern double calculate(char *exp);
extern char * infix_to_postfix(char *exp);

#endif