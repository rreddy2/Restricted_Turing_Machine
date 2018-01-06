all:	reddy_p1

reddy_p1:	reddy_p1.o
	g++  reddy_p1.o -o reddy_p1


reddy_p1.o:	reddy_p1.cpp
	g++ -c reddy_p1.cpp

clean:
	rm -f *.o reddy_p1

