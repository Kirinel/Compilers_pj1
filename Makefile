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

test101:
	./bin/ekcc -emit-ast -o ./testcases/test101.yaml ./testcases/test101.ek

test102:
	./bin/ekcc -emit-ast -o ./testcases/test102.yaml ./testcases/test102.ek

test3:
	./bin/ekcc -emit-ast -o ./testcases/test18.yaml ./testcases/test18.ek

test4:
	./bin/ekcc -emit-ast -o ./testcases/test19.yaml ./testcases/test19.ek

batch:
	./bin/ekcc -o ./testcases/test21.yaml ./testcases/test21.ek
	./bin/ekcc -o ./testcases/test22.yaml ./testcases/test22.ek
	./bin/ekcc -o ./testcases/test23.yaml ./testcases/test23.ek
	./bin/ekcc -o ./testcases/test24.yaml ./testcases/test24.ek
	./bin/ekcc -o ./testcases/test25.yaml ./testcases/test25.ek
	./bin/ekcc -o ./testcases/test26.yaml ./testcases/test26.ek
	./bin/ekcc -o ./testcases/test27.yaml ./testcases/test27.ek
	./bin/ekcc -o ./testcases/test28.yaml ./testcases/test28.ek
	./bin/ekcc -o ./testcases/test29.yaml ./testcases/test29.ek
	./bin/ekcc -o ./testcases/test30.yaml ./testcases/test30.ek
	./bin/ekcc -o ./testcases/test31.yaml ./testcases/test31.ek
	./bin/ekcc -o ./testcases/test32.yaml ./testcases/test32.ek
	./bin/ekcc -o ./testcases/test33.yaml ./testcases/test33.ek
	./bin/ekcc -o ./testcases/test34.yaml ./testcases/test34.ek
	./bin/ekcc -o ./testcases/test35.yaml ./testcases/test35.ek
	./bin/ekcc -o ./testcases/test36.yaml ./testcases/test36.ek
	./bin/ekcc -o ./testcases/test37.yaml ./testcases/test37.ek
	./bin/ekcc -o ./testcases/test38.yaml ./testcases/test38.ek
	./bin/ekcc -o ./testcases/test39.yaml ./testcases/test39.ek
	./bin/ekcc -o ./testcases/test40.yaml ./testcases/test40.ek
	./bin/ekcc -o ./testcases/test41.yaml ./testcases/test41.ek
	./bin/ekcc -o ./testcases/test42.yaml ./testcases/test42.ek
	./bin/ekcc -o ./testcases/test43.yaml ./testcases/test43.ek
	./bin/ekcc -o ./testcases/test44.yaml ./testcases/test44.ek
	./bin/ekcc -o ./testcases/test45.yaml ./testcases/test45.ek
	./bin/ekcc -o ./testcases/test46.yaml ./testcases/test46.ek
	./bin/ekcc -o ./testcases/test47.yaml ./testcases/test47.ek
	./bin/ekcc -o ./testcases/test48.yaml ./testcases/test48.ek
	./bin/ekcc -o ./testcases/test49.yaml ./testcases/test49.ek

exptype:
	./bin/ekcc -emit-ast -o ./testcases/test49.yaml ./testcases/test49.ek

clean:
	rm -f ./temp/flex.c ./temp/flex.h ./temp/bison.c ./temp/bison.h ./bin/ekcc ./testcases/*.yaml ./testcases/*.ast
