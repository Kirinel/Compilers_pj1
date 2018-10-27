CC=	clang
#CFLAGS=	-fsanitize=fuzzer,address
CFLAGS=	-g -std=gnu99 -o

#.PHONY: all clean

target:
	bison parser.y
	flex scanner.l
	$(CC) $(CFLAGS) out main.c flex.c bison.c ast.c

test1:
	./out test1.ek > test1.yaml

test2:
	./out test2.ek > test2.yaml

debug:
	./out debug.ek > debug.yaml

clean:
	rm -f flex.c flex.h bison.c bison.h out test1.yaml test2.yaml debug.yaml
