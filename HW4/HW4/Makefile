CC = g++

CFLAGS = -g -Wall -ansi

default: hw2

hw2: main.o Helper.o Poly.o Line.o
	$(CC) $(CFLAGS) -o hw2 main.o Helper.o Poly.o Line.o -lglut -lGL

main.o: main.cpp Helper.h Poly.h Line.h
	$(CC) $(CFLAGS) -c main.cpp

Helper.o: Helper.h Helper.cpp
	$(CC) $(CFLAGS) -c Helper.cpp

Poly.o: Poly.cpp Poly.h Line.h Helper.h
	$(CC) $(CFLAGS) -c Poly.cpp

Line.o: Line.cpp Line.h Helper.h
	$(CC) $(CFLAGS) -c Line.cpp

clean:
	rm -f *.o hw2
