CC=	clang
#CFLAGS=	-fsanitize=fuzzer,address
CFLAGS=	-g -std=gnu99 -o

#.PHONY: all clean

target:
	bison parser.y
	flex scanner.l
	$(CC) $(CFLAGS) ./bin/ekcc main.c flex.c bison.c ast.c

test1:
	./bin/ekcc -o ./test/test1.yaml ./test/test1.ek

test2:
	./bin/ekcc -o ./test/test2.yaml ./test/test2.ek 

debug:
	./bin/ekcc debug.ek > debug.yaml

clean:
	rm -f flex.c flex.h bison.c bison.h ./bin/ekcc ./test/test1.yaml ./test/test2.yaml debug.yaml
