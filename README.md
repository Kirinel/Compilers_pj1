## Extended-Kaleidoscope Compiler
Authors: Chuanqi Yan & Jiang Wang

### Environment and Dependencies
The program is built using `Make` and developed in Ubuntu 18.04 environment. Tested on UChicago CS department linux machines.

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
### Execute the program
Currently this program only enables the "-emit-ast" option. To generate *.yaml file from the source code *.ek, you are expected to type:

```
./bin/ekcc -emit-ast -o <output> <input>
```

for example

```
./bin/ekcc -emit-ast -o ./testcases/test10.yaml ./testcases/test10.ek
```

---
#### What has been implemented in this program?
1. -emit-ast will now generate the sequential AST.
2. Associativity and Precendence (p.4).


