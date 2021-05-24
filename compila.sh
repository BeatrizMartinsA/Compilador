flex scanner.l
bison -d -v parser.y
gcc -c *.c
gcc -o exec *.o
./exec sort.txt

rm analyze.o lex.yy.o main.o parser.output parser.tab.o symtab.o util.o cgen.o
rm lex.yy.c exec