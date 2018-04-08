#include <stdio.h>
#include "tuile.h"
#include "grille.h"
#include "test.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define clear() printf("\033[H\033[J"$)
#define gotoxy(x, y) printf("\033[%d;%dH", (x), (y))

/**
 * \brief Fonction d'affichage de la grille de la tuile que l'on est en train de poser en couleur
 * \author Thibaut Milhaud
 * \param t la Tuile que l'on est en train de positionner
 * \param g la grille de jeu
 * \param x abscisse du coin haut gauche de la tuile
 * \param y ordonnée de ce même coin
 * \param i abscisse du coin haut gauche la tuile dans la grille
 * \param j ordonnée du coin haut gauche la tuile dans la grille
 * \warning si i>n-1 ou j>n-1 on ne verra jamais la tuile à l'écran
 * \warning si la tuile dépasse de la grille je sais pas trop comment faire
 */

void affiche_plateau_2_jeu(Tuile t, Grille g, int x, int y, int i, int j)
{
	int n = g->dim;
	
	int c = t->orient % 2;

	int pose_possible = test_tout(t, i, j, g);

	/*compteurs*/
	int k;
	int k1;

	/*le + 4 est là pour prendre en compte le déclage lié à lindexation des cases*/
	gotoxy(x+4,y);
	for (k=0; k<n; k++)
	{
		printf("%i ", k);
	}
	y++;
	gotoxy(x, y);
	printf("___");
	for (k=0;k<n;k++)
	{	
		printf("__");
	}
	for (k=0; k<n; k++)
	{
		y++;
		gotoxy(x, y);
		printf("%i |", k);
		
		for (k1=0; k<n; k++)
		{
			/* on est sur la tuile */
			if (	k >= i && 
				k <= i+2-c &&
				k1 >= j &&
				k1 <= j+1+c)
			{
				if (pose_possible)
				{
					printf(KCYN "%c ", t->cases[(k-i) * (3-c) + (k1-j) * (2+c)]);
				}
				else 
				{
					printf(KRED "%c ", t->cases[(k-i) * (3-c) + (k1-j) * (2+c)]);
				}
			}
			else 
			{
				kase terrain = g->plateau[n*k+k1].c; 
				switch (terrain)
				{
					case P :
						printf("%c ", terrain);
						break;
					case F :
						printf(KGRN "%c ", terrain);
						break;
					case L :
						printf(KBLU "%c ", terrain);
						break;
					case V :
						printf(KWHT "%c ", terrain);
						break;
					case R :
						printf(KYEL "%c ", terrain);
						break;
					case U :
						printf(KMAG "%c ", terrain);
						break;
					case N :
						printf("%c ", terrain);
						break;
					default :
						printf(" haha tu croyais pouvoir me berner sqalala ");
						break;
				}
			}
		}
	}
}



					
					



