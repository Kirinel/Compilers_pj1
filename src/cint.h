#ifndef CINT_H
#define CINT_H

#define UNARY_NEG_OVERFLOW (101)
#define BINOP_ADD_OVERFLOW (102)
#define BINOP_SUB_OVERFLOW (103)
#define BINOP_MUL_OVERFLOW (104)
#define BINOP_DIV_OVERFLOW (105)


void cint_negation(int a, int *out);

void cint_addition(int a, int b, int *sum);

void cint_subtraction(int a, int b, int *diff);

void cint_multiplication(int a, int b, int *product);

void cint_division(int a, int b, int *quotient);

#endif
