all: VMtranslator

VMtranslator: VMtranslator.o codewriter.o
	gcc -o VMtranslator VMtranslator.o codewriter.o

VMtranslator.o: VMtranslator.c codewriter.h
	gcc -c VMtranslator.c

codewriter.o: codewriter.c codewriter.h
	gcc -c codewriter.c

clean: 
	rm -rf *.o VMtranslator
