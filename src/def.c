#include "def.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
/* type_to_exptype: Given a <type> node, convert it into a enum exp_tag
 */
enum exp_type type_to_exptype(Node *n_type)
{
  assert(n_type->tag == T_TYPE);

  switch (n_type->node_type.type_tag) {
    case TYPE_INT:
      return INT;
    case TYPE_CINT:
      return CINT;
    case TYPE_FLOAT:
      return FLOAT;
    case TYPE_SFLOAT:
      return SFLOAT;
    case TYPE_VOID:
      return VOID;
    default:
      // ref type
      return type_to_exptype(n_type->node_type.type);
  }
  // return;
}

/* type_to_llvmtype: Given a <type> node, convert it into a enum exp_tag
 */
enum llvm_type type_to_llvmtype(Node *n_type)
{
  assert(n_type->tag == T_TYPE);

  switch (n_type->node_type.type_tag) {
    case TYPE_INT:
      return LLVM_INT;
    case TYPE_CINT:
      return LLVM_CINT;
    case TYPE_FLOAT:
      return LLVM_FLOAT;
    case TYPE_SFLOAT:
      return LLVM_SFLOAT;
    case TYPE_VOID:
      return LLVM_VOID;
    case TYPE_REF:
      switch (n_type->node_type.type->node_type.type_tag) {
        case TYPE_INT:
          return LLVM_REF_INT;
        case TYPE_CINT:
          return LLVM_REF_CINT;
        case TYPE_FLOAT:
          return LLVM_REF_FLOAT;
        case TYPE_SFLOAT:
          return LLVM_REF_SFLOAT;
        default:
          fprintf(stderr, "unknown type.\n");
          exit(99);
      }
    break;
  case TYPE_NOALIAS_REF:
    switch (n_type->node_type.type->node_type.type_tag) {
      case TYPE_INT:
        return LLVM_NOALIAS_REF_INT;
      case TYPE_CINT:
        return LLVM_NOALIAS_REF_CINT;
      case TYPE_FLOAT:
        return LLVM_NOALIAS_REF_FLOAT;
      case TYPE_SFLOAT:
        return LLVM_NOALIAS_REF_SFLOAT;
      default:
        fprintf(stderr, "unknown type.\n");
        exit(99);
    }
  // return;
  }
}
