## Extended-Kaleidoscope Compiler
Authors: Chuanqi Yan & Jiang Wang

(Lastest Update on Nov 25th 8:00am)

### Environment and Dependencies
The program is built using `Make` and developed in Ubuntu 18.04 environment (with llvm 6.0.0). This program is not tested on the linux cs machines due to lack of LLVM distribution.

### Reminder (New)
If you prompt `make` and the program fails to program, please make sure you have the empty directories named `bin` and `temp` in your current working directory. These directories are used to hold the executable and temporary files.

---
1. This project underwent a large redesign after the second assignment. Though most of the functionality are restored after the redesign, the program will not check the correctness when you call a function. Please make sure the types and number of arguments match with the function signature you are trying to call.

2. JIT is the only option to compile and run the program.

3. No terminators are allowed in the while loop. That is, the while loop must be branched to the `cond` basic block. However, terminators are allowed in if statements.

4. Commandline arguments are stored in the program using vectors, and therefore extern functions getarg and getargf have bodies.

5. Based on the class discussion, LLVM C API does not support math flags for floating-point numbers, so sfloat is treated as float in this program.

---

#### Example of the run the executable (preferred way)

./bin/ekcc -emit-llvm -jit -o ./tests/test02.ll ./tests/test02.ek [commandline arguments]

#####*System Requirements*:

clang & llvm (v-6.0.0 or higher)

bison (v-3.0.4, please note that bison 2.X cannot correctly compiles this program)

flex (v-2.6.4)

---
### Build the program
This program can be simply built using the command:

```
make
```

If you have seen any linking issues when building this project, remember to create a new directory called "bin", which is needed for generating the executable:

```
mkdir bin
```

To show the help page of the program, you can type:

```
make help
```

The program executable is

```
./bin/ekcc
```

---
### Execute the program (deprecated)
Currently this program only enables the "-emit-ast" option. To generate \*.yaml file from the source code \*.ek, you are expected to type:

```
./bin/ekcc -emit-ast -o <output> <input>
```

for example

```
./bin/ekcc -emit-ast -o ./testcases/test10.yaml ./testcases/test10.ek
```

---
### Error detection:
Updated on 11/4, this program now enables basic type checking and will print error information to `stderr`.
1. You cannot declare a variable with void type. Functions cannot be declared with arguments of void type. (code: 001)
2. A ref type cannot contain a ref type or void type. This means variable declarations, the argument of function declarations, and the prototype of extern functions, cannot contain [ref ref] type or [ref void] type. (code: 002)
3. All functions must be declared before use, meaning you cannot call a function before it is declared or defined. (code: 003)
4. A function cannot be declared with a ref return type. (code: 004)
5. Variables initializer must be a variable. This applies when you declare a local variable, or pass a parameter by making a function call. The later means the following is not allowed. (code: 005)

*The following functionality is no longer available with the new update*

```
def void inc(ref int $n) {

}

def int run() {
  inc(1); #invalid
}
```

6. The source code must have a run function. The run function must have a return type that is not a ref type. The run function must take no arguments. (code: 006)
7. Disable multiple declarations of variables with same alias. (code: 008)
8. When calling a function, the number of arguments must match with the function prototype exactly.(code: 009)

---
#### What has been implemented in this program?
1. -emit-ast will now generate the sequential AST.
2. Associativity and Precendence (p.4).
3. Basic Type checking (Nov 4th).
4. Every expression now has a field expr_type that records their type (Nov 4th).
5. JIT compilation and LLVM IR generation (Nov 24th).
