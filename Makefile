CC=	clang

target:
	bison parser.y
	flex scanner.l
	$(CC) -std=gnu99 -o out main.c flex.c bison.c ast.c

test:
	./out test.txt

clean:
	rm -f flex.c flex.h bison.c bison.h out
