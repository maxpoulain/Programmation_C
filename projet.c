#include "projet.h"

/* Pour compiler : make projet */

/* Fonction de récupération du labyrinthe présent dans un fichier txt */
void tableau2D(lab *a)
{	
	FILE *fichier;  
	fichier=fopen("Fichier_test.txt","r");  
	if(fichier == NULL)
	{
		exit(-1);  
	}

	fscanf(fichier,"%u %u %u %u %u %u",&a->nblig,&a->nbcol,&a->posentx,&a->posenty,&a->possortx,&a->possorty);
	if(a == NULL)
	{
		exit(-1);  
	}

	a->tab=malloc(sizeof(*a->tab)*(a->nblig));  
	if (a->tab == NULL)
	{
		exit(-1);  
	}

	unsigned i;  
	for(i=0;i<a->nblig;i++)
	{
		a->tab[i]=malloc(sizeof(a->tab[i])*(a->nbcol));  
		if (a->tab[i] == NULL)
		{
			exit(-1);
		}
	}

	unsigned j,k;
	for(j=0;j<a->nblig;j++)
	{
		for (k=0;k<a->nbcol;k++)
		{
			fscanf(fichier,"%hu",&a->tab[j][k]);			
		}
	}
	fclose(fichier);
}

/* Fonction qui affiche les nombres correspondant à chaque case du labyrinthe*/
void affiche(lab *a)
{
	unsigned i,j;
	for (i=0;i<a->nblig;i++)
	{
		for (j=0;j<a->nbcol;j++)
		{
			printf("%hu ",a->tab[i][j]);
		}

		printf("\n");
	}
}

/* Fonction d'affichage du labyrinthe */
void affichelab(lab *a)
{
	int i=0,k=0,l=0;
	for(i=0;i<a->nbcol;i++)
	{
		printf("+--");
	}
	printf("+\n");
	for(l=0;l<a->nblig;l++)
	{	
		printf("|");
		for(k=0;k<a->nbcol;k++)
		{
			int j=0;
			unsigned bit;
			for (j=0;j<4;j++)
			{
				bit=a->tab[l][k]>>j&1;
				if(bit == 1 && j==2){
					if(k!=(a->nbcol)-1 && (a->tab[l][k+1]>>0&1)==1 && ((a->tab[l][k]>>8&1)==0))
					{
						printf("  |");
					} 
					else if(k!=(a->nbcol)-1 && (a->tab[l][k+1]>>0&1)==1 && (a->tab[l][k]>>8&1)==1)
					{
						printf(" *|");
					}	
					if(k==(a->nbcol)-1 && (a->tab[l][k]>>8&1)==0)
					{
						printf("  |\n");
					}
					else if(k==(a->nbcol)-1 && (a->tab[l][k]>>8&1)==1)
					{
						printf(" *|\n");
					}	
				}
				else if(bit == 0 && j==2 && (a->tab[l][k]>>8&1)==0)
				{
					printf("   ");
				}
				else if(bit == 0 && j==2 && (a->tab[l][k]>>8&1)==1)
				{
					printf(" * ");
				}	
			}
		}
		for(k=0;k<a->nbcol;k++)
		{
			int j=0;
			unsigned bit;
			for (j=0;j<4;j++)
			{
				bit=a->tab[l][k]>>j&1;
				if(bit == 1 && j==1){
					if(l!=(a->nblig)-1 && (a->tab[l+1][k]>>3&1)==1)
					{
						printf("+--");
					}
					if(l==(a->nblig)-1){
						printf("+--");
					}
				}else if(bit == 0 && j==1)
				{
					printf("+  ");
				}
			}
		}
		printf("+\n");
	}
	printf("\n");
}

/* Fonction qui met les murs virtuels */
void murvirtuel(lab *a)
{
	int i,j,k;
	unsigned bit=0;
	for(j=0;j<a->nblig;j++)
	{	
		for(k=0;k<a->nbcol;k++)
		{
			for(i=0;i<4;i++)
			{	
				bit=a->tab[j][k]>>i&1;
				if(bit==1 && i==0)
				{
					a->tab[j][k]=a->tab[j][k]^16;
				}
				if(bit==1 && i==1)
				{
					a->tab[j][k]=a->tab[j][k]^32;
				}
				if(bit==1 && i==2)
				{
					a->tab[j][k]=a->tab[j][k]^64;
				}
				if(bit==1 && i==3)
				{
					a->tab[j][k]=a->tab[j][k]^128;
				}
			}
		}
	}
}


/* Fonction qui compte le nombre de sortie possible (la ou le bit est à 0) et le retourne pour la recherche du chemin le plus court  */
int croisement(lab *a,unsigned ligne,unsigned col, int entre)
{
	int i,compt=0,bit,tmp=((entre+1)%4)+3;
	entre=(entre%4)+4;
	for(i=tmp+1;i<8;i++)
	{	
		if(i != entre)
		{	
			bit=a->tab[ligne][col]>>i&1;
			if(bit==0)
			{
				compt++;
			}
		}
		if(i==7)
		{
			i=i-4;
		}
		if(i==tmp)
		{
			break;
		}	
	}
	return compt;
}

/* Fonction qui compte le nombre de sortie possible (la ou le bit est à 0) et le retourne pour la recherche de chemin */ 
int inter(lab *a,unsigned ligne,unsigned col)
{
	int i,compt=0,bit;
	for(i=4;i<8;i++)
	{
		bit=a->tab[ligne][col]>>i&1;
		if(bit==0)
		{
			compt++;
		}	
	}	
	return compt;
}

/* Fonction qui génére aléatoirement un labyrinthe */
void aleatoire (lab *a)
{
	printf("Veuillez entrer le nombre de lignes et de colonnes\n");
	scanf("%u %u",&a->nblig,&a->nbcol);
	a->posentx=0;
	a->posenty=0;
	a->possortx=(a->nblig-1);
	a->possorty=(a->nbcol-1);
	if(a == NULL)
	{
		exit(-1);
	}
	a->tab=malloc(sizeof(*a->tab)*(a->nblig));
	if(a->tab == NULL)
	{
		exit(-1);
	}
	int i=0,j=0,alea1,alea2;
	for (i = 0; i < a->nblig; i++)
	{
		a->tab[i]=malloc(sizeof(a->tab[i])*(a->nbcol));
		if(a->tab[i] == NULL)
		{
			exit(-1);
		}	
	}
	for (i = 0; i < a->nblig; i++)
	{
		for (j = 0; j < a->nbcol; j++)
		{
			a->tab[i][j]=15;
		}
	}
	for (i = 0; i < a->nblig; i++)
	{
		for (j = 0; j < a->nbcol; j++)
		{	
			alea1=rand()%4;
			alea2=rand()%4;
			if((alea1==1 || alea1==2) && i<a->nblig-1)
			{
				a->tab[i][j]=a->tab[i][j]^2;
				a->tab[i+1][j]=a->tab[i+1][j]^8;
			}
			if((alea2==1 || alea2==2) && j<a->nbcol-1)
			{
				a->tab[i][j]=a->tab[i][j]^4;
				a->tab[i][j+1]=a->tab[i][j+1]^1;
			}
		}
	}		
}


/* Fonction qui recherche un parcours */
void recherche(lab *a)
{
	int i;	
	murvirtuel(a);
	a->posx=a->posentx;
	a->posy=a->posenty;
	int cpt=0;
	i=2;
	while(1)
	{	
		if((a->posx==a->possortx) &&(a->posy==a->possorty))
		{	
			a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
			affichelab(a);
			printf("WIN\n");
			break;
		}
		cpt=0;
		for(i=(((i+1)%4)+4);i>3;i--)
		{	

			unsigned bit=bit=a->tab[a->posx][a->posy]>>i&1;
			if(bit==1)
			{
				cpt++;
			}
			if(bit==0 && i==4)
			{	
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^16;
				if((a->tab[a->posx][a->posy] >> 8 & 1) && (inter(a,a->posx,a->posy)==0))
				{	
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posy--;
					break;
				}
				else if(a->tab[a->posx][a->posy] >> 8 & 1)
				{
					a->posy--;
					break;

				}
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
				a->posy--;
				break;

			}
			if(bit==0 && i==5)
			{	
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^32;
				if((a->tab[a->posx][a->posy] >> 8 & 1) && (inter(a,a->posx,a->posy)==0))
				{
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posx++;	
					break;	
				}
				else if(a->tab[a->posx][a->posy] >> 8 & 1)
				{
					a->posx++;
					break;
				}	
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
				a->posx++;	
				break;			
			}
			if(bit==0 && i==6)
			{	
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^64;
				if((a->tab[a->posx][a->posy] >> 8 & 1) && (inter(a,a->posx,a->posy)==0))
				{
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posy++;
					break;
				}else if(a->tab[a->posx][a->posy] >> 8 & 1)
				{
					a->posy++;
					break;
				}
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;	
				a->posy++;
				break;
			}
			if(bit==0 && i==7)
			{	
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^128;
				if(a->tab[a->posx][a->posy] >> 8 & 1 && (inter(a,a->posx,a->posy)==0))
				{
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posx=a->posx-1;
					break;
				}else if(a->tab[a->posx][a->posy] >> 8 & 1)
				{
					a->posx=a->posx-1;
					break;
				}
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;	
				a->posx=a->posx-1;
				break;	
			}
			if(cpt==3)
			{
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
			}
			if(cpt == 4)
			{
				printf("bloqué !!\n");
			}
			if(i==4)
			{
				i+=4;
			}	
		}
	}
}

/* Fonction qui calcul les distances */
void distance(lab *a, int distance[a->nblig][a->nbcol]){
	int i,j;
	for(i=0;i<a->nblig;i++)
	{
		for(j=0;j<a->nbcol;j++)
		{
			distance[i][j]=sqrt(pow((a->possortx - i),2) + pow(((a->possorty - j)),2));
		}
	}
}

/* Fonction qui recherche le plus court chemin */
void recherchepluscourt(lab *a)
{

	int dist[a->nblig][a->nbcol]; 
	int i=2,cpt=0,flag=0,sortie=0,entre=i%4+6;
	distance(a,dist);
	murvirtuel(a);
	a->posx=a->posentx;
	a->posy=a->posenty;

	while(1)
	{	
		if((a->tab[a->posx][a->posy]>>4&1)==1 && (a->tab[a->posx][a->posy]>>5&1)==1 && (a->tab[a->posx][a->posy]>>6&1)==1 && (a->tab[a->posx][a->posy]>>7&1)==1)
		{
			printf("PAS DE SORTIE\n");
			break;
		}
		if((a->posx==a->possortx) &&(a->posy==a->possorty))
		{	
			a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
			affichelab(a);
			printf("WIN\n");
			break;
		}
		cpt=0;
		if(flag==1)
		{
			entre=i%4+6;
			flag=0;
		}
		for(i=(((i+1)%4)+4);i>3;i--)
		{	
			unsigned bit=bit=a->tab[a->posx][a->posy]>>i&1;
			if(bit==1)
			{
				cpt++;
			}
			//printf("bit[%d]:%u\n",i,bit);l
			if(bit==0 && i==4)
			{	
				sortie=croisement(a,a->posx,a->posy,entre);
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^16;
				if(sortie ==1)
				{	
					flag=1;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posy--;
					break;
				}
				else if((a->tab[a->posx][a->posy]>>7&1)==0 && (dist[a->posx][a->posy-1]>dist[a->posx-1][a->posy]))
				{

				}
				else if((a->tab[a->posx][a->posy]>>6&1)==0 && (dist[a->posx][a->posy-1]>dist[a->posx][a->posy+1]))
				{

				}
				else if((a->tab[a->posx][a->posy]>>5&1)==0 && (dist[a->posx][a->posy-1]>dist[a->posx+1][a->posy]))
				{

				}
				else 
				{	
					flag=1;
					if((a->tab[a->posx][a->posy]>> 8 & 1)==1 && croisement(a,a->posx,a->posy,entre)==0)
					{
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
						a->posy--;
						break;
					}
					else if((a->tab[a->posx][a->posy]>> 8 & 1)==1)
					{
						a->posy--;
						break;
					}
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posy--;
					break;
				}	
			}

			if(bit==0 && i==5)
			{	
				sortie=croisement(a,a->posx,a->posy,entre);
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^32;
				if(sortie==1)
				{	
					flag=1;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posx++;
					break;
				}
				else if((a->tab[a->posx][a->posy]>>4&1)==0 && (dist[a->posx+1][a->posy]>dist[a->posx][a->posy-1]))
				{

				}
				else if((a->tab[a->posx][a->posy]>>6&1)==0 && (dist[a->posx+1][a->posy]>dist[a->posx][a->posy+1]))
				{

				}
				else if((a->tab[a->posx][a->posy]>>7&1)==0 && (dist[a->posx+1][a->posy]>dist[a->posx-1][a->posy]))
				{

				}
				else 
				{
					flag=1;
					if((a->tab[a->posx][a->posy]>> 8 & 1)==1 && croisement(a,a->posx,a->posy,entre)==0)
					{
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
						a->posx++;
						break;
					}
					else if((a->tab[a->posx][a->posy]>> 8 & 1)==1)
					{
						a->posx++;
						break;
					}
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posx++;
					break;	
				}
			}

			if(bit==0 && i==6)
			{	
				sortie=croisement(a,a->posx,a->posy,entre);
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^64;
				if(sortie==1)
				{	
					flag=1;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posy++;
					break;
				}
				else if((a->tab[a->posx][a->posy]>>4&1)==0 && (dist[a->posx][a->posy+1]>dist[a->posx][a->posy-1]))
				{	

				}
				else if((a->tab[a->posx][a->posy]>>5&1)==0 && (dist[a->posx][a->posy+1]>dist[a->posx+1][a->posy]))
				{	

				}
				else if((a->tab[a->posx][a->posy]>>7&1)==0 && (dist[a->posx][a->posy+1]>dist[a->posx-1][a->posy]))
				{	

				}
				else 
				{
					flag=1;
					if((a->tab[a->posx][a->posy]>> 8 & 1)==1 && croisement(a,a->posx,a->posy,entre)==0)
					{
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
						a->posy++;
						break;
					}
					else if((a->tab[a->posx][a->posy]>> 8 & 1)==1)
					{
						a->posy++;
						break;
					}
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posy++;
					break;	
				}
			}

			if(bit==0 && i==7)
			{	
				sortie=croisement(a,a->posx,a->posy,entre);
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^128;
				if(sortie==1)
				{	
					flag=1;
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posx--;
					break;
				}
				else if((a->tab[a->posx][a->posy]>>4&1)==0 && (dist[a->posx-1][a->posy]>dist[a->posx][a->posy-1]))
				{


				}
				else if((a->tab[a->posx][a->posy]>>5&1)==0 && (dist[a->posx-1][a->posy]>dist[a->posx+1][a->posy]))
				{

				}
				else if((a->tab[a->posx][a->posy]>>6&1)==0 && (dist[a->posx-1][a->posy]>dist[a->posx][a->posy+1]))
				{

				}
				else 
				{
					flag=1;
					if((a->tab[a->posx][a->posy]>> 8 & 1)==1 && croisement(a,a->posx,a->posy,entre)==0)
					{
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
						a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
						a->posx--;
						break;
					}
					else if((a->tab[a->posx][a->posy]>> 8 & 1)==1)
					{
						a->posx--;
						break;
					}
					a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
					a->posx--;
					break;
				}	
			}	

			if(flag==1 && bit==0)
			{
				break;
			}

			if(cpt==3)
			{
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^256;
				a->tab[a->posx][a->posy]=a->tab[a->posx][a->posy]^512;
			}

			if(i==4)
			{
				i+=4;
			}	
		}
	}
}

/* Fonction de désallocation */
void desalloc(lab *a)
{
	unsigned i;
	for (i=0;i<a->nblig;i++)
	{
		free(a->tab[i]);
	}
	free(a);
}

/* Fonction main */
int main ()
{	
	int choix;
	printf("Tapez 1 pour trouver un chemin\n");
	printf("Tapez 2 pour le chemin le plus court\n");
	printf("Tapez 3 pour générer un labyrinthe aléatoirement\n");
	scanf("%d",&choix);
	if(choix==1)
	{	
		lab *a=malloc(sizeof(lab));
		tableau2D(a);
		affiche(a);
		affichelab(a);
		recherche(a);
		desalloc(a);
	}
	else if(choix==2)
	{	
		lab *a=malloc(sizeof(lab));
		tableau2D(a);
		affiche(a);
		affichelab(a);
		recherchepluscourt(a);
		desalloc(a);	
	}
	else if(choix==3)
	{
		lab *a=malloc(sizeof(lab));
		srand(time(NULL));
		aleatoire(a);
		affichelab(a);
		recherchepluscourt(a);
		desalloc(a);
	}
	else
	{
		printf("Vous vous êtes trompé, vous n'avez pas tapez sur 1, 2 ou 3, le programme a quitté.\n");
		printf("Veuillez le relancer le programme !\n");
		exit(-1);
	}


	return(0);
}
