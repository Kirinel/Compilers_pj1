CC=	clang
CFLAGS=	-g -O1 -o

target:
	bison parser.y
	flex scanner.l
	$(CC) $(CFLAGS) ./bin/ekcc main.c flex.c bison.c ast.c

help:
	./bin/ekcc -h

test1:
	./bin/ekcc -emit-ast -o ./testcases/test10.yaml ./testcases/test10.ek

test2:
	./bin/ekcc -emit-ast -o ./testcases/test16.yaml ./testcases/test16.ek

clean:
	rm -f flex.c flex.h bison.c bison.h ./bin/ekcc ./testcases/*.yaml ./testcases/*.ast
