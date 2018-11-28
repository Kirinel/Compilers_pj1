#include "compile.h"

char **argVector;

extern "C" {
  int getarg(int number) {
    return atoi(argVector[number+1]);
  }

  int getargf(int number) {
    return atof(argVector[number+1]);
  }
}

int main(int arg, char **argv) {

  FunctionJIT = Fetch the run function's address or callable object.
  int retVal = FunctionJIT();

  return retVal;
}

#include <something that has C API's magic unwrap function>
// llvm::unrap(LLVMModuleRef

extern "C" {
  void AddNoAliasToParam(LLVMModuleRef Module, char *FunctionName, int ParamNum) {
    llvm::Module *M = llvm::unwrap(CM);
    llvm::Function *F = M.getfunction(FunctionName);

    Function *fun = dynamic_cast<Function>(Fn);
    fun->addAttribute(ParamNum, Attribute::NoAlias);
  }
}
