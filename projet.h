#ifndef laby
#define laby
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct lab_{
	unsigned short **tab;
	unsigned int  nblig;
	unsigned int  nbcol;
	unsigned int  posx;
	unsigned int  posy;
	unsigned int  posentx;
	unsigned int  posenty;
	unsigned int  possortx;
	unsigned int  possorty;
} lab;

void tableau2D(lab *a);
void affiche(lab *a);
void desalloc(lab *a);
void affichelab(lab *a);
void aleatoire(lab *a);
void murvirtuel(lab *a);
int inter(lab *a,unsigned ligne,unsigned col);
int croisement(lab *a,unsigned ligne,unsigned col, int entre);
void recherche(lab *a);
void distance(lab *a, int distance[a->nblig][a->nbcol]);
void recherchepluscourt(lab *a);

#endif