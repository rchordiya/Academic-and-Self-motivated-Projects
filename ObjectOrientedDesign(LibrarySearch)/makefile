output: Book.o Periodic.o Video.o Film.o Media.o Main.o SearchEngine.o
	g++ -g Book.o Periodic.o Video.o Film.o Media.o Main.o SearchEngine.o -o output

Main.o: Main.cpp SearchEngine.h Media.h Book.h Video.h Periodic.h Film.h
	g++ -g -c -std=c++11 Main.cpp

SearchEngine.o: SearchEngine.cpp SearchEngine.h
	g++ -g -c -std=c++11  SearchEngine.cpp

Media.o: Media.cpp Media.h
	g++ -g -c -std=c++11 Media.cpp

Book.o: Book.cpp Book.h
	g++ -g -c -std=c++11 Book.cpp

Periodic.o: Periodic.cpp Periodic.h
	g++ -g -c -std=c++11 Periodic.cpp

Video.o: Video.cpp Video.h
	g++ -g -c -std=c++11 Video.cpp

Film.o: Film.cpp Film.h
	g++ -g -c -std=c++11 Film.cpp

clean:
	rm -f *.o output
