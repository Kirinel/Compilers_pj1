#include "cint.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Check the overflow of unary negation
 *  a (int32): value of the expression.
 *  out (int32 *): pointer to the value after negation.
 */
void cint_negation(int a, int *out) {
  int zero = 0;
  int overflow = __builtin_ssub_overflow(zero, a, out);

  if (overflow) {
    fprintf(stderr, "error: overflow at unary negation (%d) (code: %d).\n", a, UNARY_NEG_OVERFLOW);
    exit(UNARY_NEG_OVERFLOW);
  }
}

/* Check the overflow of binary addition
 *  a (int32): value of the expression on the left hand side
 *  b (int32): value of the expression on the right hand side
 *  sum (int32 *): pointer to the sum of two values
 */
void cint_addition(int a, int b, int *sum) {
  int overflow = __builtin_sadd_overflow(a, b, sum);

  if (overflow) {
    fprintf(stderr, "error: overflow at binary addition (%d, %d) (code: %d).\n", a, b, BINOP_ADD_OVERFLOW);
    exit(BINOP_ADD_OVERFLOW);
  }
}

/* Check the overflow of binary subtraction
 *  a (int32): value of the expression on the left hand side
 *  b (int32): value of the expression on the right hand side
 *  diff (int32 *): pointer to the difference of two values
 */
void cint_subtraction(int a, int b, int *diff) {
  int overflow = __builtin_ssub_overflow(a, b, diff);

  if (overflow) {
    fprintf(stderr, "error: overflow at binary subtraction (%d, %d) (code: %d).\n", a, b, BINOP_SUB_OVERFLOW);
    exit(BINOP_SUB_OVERFLOW);
  }
}

/* Check the overflow of binary multiplication
 *  a (int32): value of the expression on the left hand side
 *  b (int32): value of the expression on the right hand side
 *  product (int32 *): pointer to the product of the two values
 */
void cint_multiplication(int a, int b, int *product) {
  int overflow = __builtin_smul_overflow(a, b, product);

  if (overflow) {
    fprintf(stderr, "error: overflow at binary multiplication (%d, %d) (code: %d).\n", a, b, BINOP_MUL_OVERFLOW);
    exit(BINOP_MUL_OVERFLOW);
  }
}

/* Check the overflow of binary division
 *  a (int32): value of the expression on the left hand side
 *  b (int32): value of the expression on the right hand side
 *  quotient (int32 *): pointer to the product of quotient of the two values
 */
void cint_division(int a, int b, int *quotient) {
  // Check the right hand side is 0
  if (!b) {
    fprintf(stderr, "error: division by 0 (code: %d).\n", BINOP_DIV_OVERFLOW);
    exit(BINOP_DIV_OVERFLOW);
  }
  if (a==INT_MIN && b==-1) {
    fprintf(stderr, "error: overflow at binary division (%d, %d) (code: %d).\n", a, b, BINOP_DIV_OVERFLOW);
    exit(BINOP_DIV_OVERFLOW);
  }
}

// int warn(int x) {
//   // int x = 2147483640;
//   // int y = 10;
//   // int sum = x + y;
//   // int overflow = __builtin_sadd_overflow(x, y, &sum);
//
//   int zero = 0;
//   int m = -2147483648;
//   int diff = zero - m;
//   int overflow = __builtin_ssub_overflow(zero, m, &diff);
//
//   fprintf(stderr, "Trapped! %d\n", overflow);
//
//   return 0;
// }
