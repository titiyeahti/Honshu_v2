/**
 * \file ytest.c
 * Ce fichier implémente les fonctions de tests définies dans \b ytest.h
 */

#include "test.h"
#include "grille.h"
#include "tuile.h"

int is_ingrid(Tuile t, int i, int j, Grille g)
{
	int n = g->dim;
	if (t->orient % 2 == 0)
		return (i<n-1 && j<n-2);
	else 
		return (i<n-2 && j<n-1);
}

int is_connected(Tuile t, int i, int j, Grille g)
{
	int k;
	int l;
	int n = g->dim;
	/* Si on est orienté Nord ou Sud (horizontal) */
	if (t->orient % 2 == 0)
	{
		for (k = i; k < i + 2; k++)
		{
			for (l = j; l < j + 3; l++)
			{
				if ((g->plateau)[n * k + l].no_tuile != 0)
					return 1;
			}
	
		}
	}
	/* Sinon (vertical) */
	else 
	{
		for (k=i; k<i+3; k++)
		{
			for (l=j; l<j+2; l++)
			{
				if ((g->plateau)[n*k+l].no_tuile != 0)
					return 1;
			}
		}
	}
	return 0;
}

int dont_fill_lake(Tuile t, int i, int j, Grille g)
{
	int k;
	int l;
	int n = g->dim;
	if (t->orient % 2 == 0)
	{
		for (k=i; k<i+2; k++)
		{
			for (l=j; l<j+3; l++)
			{
				if ((g->plateau)[n*k+l].c == L)
					return 0;
			}
	
		}
	}
	else 
	{
		for (k=i; k<i+3; k++)
		{
			for (l=j; l<j+2; l++)
			{
				if ((g->plateau)[n*k+l].c == L)
					return 0;
			}
		}
	}
	return 1;
}

int verifier_non_couvrement_total(Tuile t, int pi, int pj, Grille g)
{
	int x = 0, i = 0, j = 0, tmpCount = 0;
	int n = g->dim;
	/* int clone[n*n]; */
	int* clone = malloc(n * n * sizeof(int));
	int nb = g->nb_tuiles;

	/* cloner dans un tableau d'ids */

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			clone[n * i + j] = (g->plateau)[n * i + j].no_tuile;
		}
	}
	int k,l;
	
	/* placer les 6 nouveaux ids dans idTab[][] dans clone */
	if (t->orient % 2 == 0)
	{
		for (k = pi; k < pi + 2; k++)
		{
			for (l = pj; l < pj + 3; l++)
			{
				clone[k * n + l] = nb + 1;
			}
		}
	}
	else
	{
		for (k = pi; k < pi + 3; k++)
		{
			for (l = pj; l < pj + 2; l++)
			{
				clone[k * n + l] = nb + 1;
			}
		}
	}
	/* verifier les ids */
	for (x = 0; x < nb+1; x++)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (clone[n * i + j] == x)
				{
					tmpCount++;
				}
			}
		}
		/*printf("\nmdr : %d ,%d\n", i, tmpCount);*/
		if (tmpCount == 0)
		{
			free(clone);
			return 0;
		}
		tmpCount = 0;
	}
	free(clone);
	return 1;
}

int test_tout(Tuile t, int i, int j, Grille g)
{
	return (is_ingrid(t, i, j, g) &&
			is_connected(t, i, j, g) &&
			dont_fill_lake(t, i, j, g) &&
			verifier_non_couvrement_total(t, i, j, g));
}
