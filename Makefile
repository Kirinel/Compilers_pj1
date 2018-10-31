CC=	clang
CFLAGS=	-g -O1 -o

target:
	bison ./src/parser.y
	flex ./src/scanner.l
	$(CC) $(CFLAGS) ./bin/ekcc ./src/main.c ./temp/flex.c ./temp/bison.c ./src/ast.c ./src/symbol_table.c ./src/tools.c

help:
	./bin/ekcc -h

test1:
	./bin/ekcc -emit-ast -o ./testcases/test10.yaml ./testcases/test10.ek

test2:
	./bin/ekcc -emit-ast -o ./testcases/test16.yaml ./testcases/test16.ek

test3:
	./bin/ekcc -emit-ast -o ./testcases/test18.yaml ./testcases/test18.ek

clean:
	rm -f ./temp/flex.c ./temp/flex.h ./temp/bison.c ./temp/bison.h ./bin/ekcc ./testcases/*.yaml ./testcases/*.ast
