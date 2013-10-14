CFLAGS=-O0 -g -pedantic -Werror -Wall

cscd340_f13_hw3:  cscd340_f13_hw3.o linkedList.o alias.o

clean:
	rm -f *.o hw3
