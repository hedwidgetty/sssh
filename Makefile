CFLAGS=-O0 -g -pedantic -Werror -Wall

cscd340_f13_hw3:  cscd340_f13_hw3.o linkedList.o alias.o

linkedList.o: linkedList.h

alias.o: alias.h

clean:
	rm -f *.o cscd340_f13_hw3
