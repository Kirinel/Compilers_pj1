CC=	clang

target:
	bison parser.y
	flex scanner.l
	$(CC) -std=gnu99 -o out main.c flex.c bison.c ast.c

test1:
	./out test1.ek

test2:
	./out test2.ek

clean:
	rm -f flex.c flex.h bison.c bison.h out
