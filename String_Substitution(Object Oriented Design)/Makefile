#Compiler
CC = g++

#Compiler flags
CFLAGS = -c -std=c++11 -g -Wall

#target name
all : a.out

a.out : Streamsource.o Bufferfile.o main.o
	$(CC) Streamsource.o Bufferfile.o main.o -o a.out

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Streamsource.o : Streamsource.cpp
	$(CC) $(CFLAGS) Streamsource.cpp

Bufferfile.o : Bufferfile.cpp
	$(CC) $(CFLAGS) Bufferfile.cpp

clean:
	rm *.o a.out
