projet.o : projet.c projet.h
	gcc -g -Wall -c projet.c -lm

projet : projet.o
	gcc -g -Wall projet.o -o projet -lm

clean : 
	rm *.o