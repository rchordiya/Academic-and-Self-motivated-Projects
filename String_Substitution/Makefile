#Compiler
CC = g++

#Compiler flags
CFLAGS = -c -std=c++11 -g -Wall

#target name
all : a.out

a.out : customfstream.o main.o
	$(CC) customfstream.o main.o -o a.out

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

customfstream.o : customfstream.cpp
	$(CC) $(CFLAGS) customfstream.cpp


clean:
	rm *.o a.out
