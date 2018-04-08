/**
 * \file grille.c
 * Ce fichier implémente les fonctions définies dans \b grille.h
 */

#include "grille.h"
#include "tuile.h"
#include "test.h"


Grille nouvelle_grille(int n)
{
	if (n <= 0)
		return NULL;

	int i;
	Grille g = malloc(sizeof(grille));
	g->plateau = malloc(n*n*sizeof(kase_n));
	g->dim = n;
	g->nb_tuiles = 0;
	for (i = 0; i < n * n; i++)
	{
		(g->plateau)[i].no_tuile = 0;
		(g->plateau)[i].c = N;
	}

	return g;
}

Grille nouvelle_grille_avec_tuile(int n)
{
	if (n <= 0)
		return NULL;

	Grille g = nouvelle_grille(n);
	Tuile t = rand_tuile();
	ajoute_tuile(n / 2 - 1, n / 2 - 1, t, g);
	return g;
}

void free_grille(Grille g)
{
	if (g == NULL)
		return;
		
	int i;
	for (i=0; i<g->nb_tuiles; i++)
	{
		/* Si la tuile n'a pas été chargée depuis un fichier, on la libère ici. */
		if (!((((g->tuiles)[i])->t)->fromFile))
			free(((g->tuiles)[i])->t);

		free((g->tuiles)[i]);
	}
	free(g->plateau);
	free(g);
}

kase lecture_case(int i, int j, Grille g)
{
	if (i >= g->dim)
		return 0;
	else if (j >= g->dim)
		return 0;

	return ((g->plateau)[g->dim*i+j]).c;
}

kase lecture_case_brut(int k, Grille g)
{
	if (k >= g->dim * g->dim)
		return 0;

	return (g->plateau)[k].c;
}

void print_grille(Grille g)
{
	int i;
	int j;
	int n = g->dim;
	printf("   ");
	for (j = 0; j < n; j++)
		printf("%3d", j);
	printf("\n");

	for (i=0;i<n;i++)
	{
		printf("%3d", i);
		for (j=0;j<n;j++)
		{
			printf("  %c",((g->plateau)[n*i+j]).c);
		}
		printf("\n");
	}
	printf("\n");
}

void print_politique(Grille g)
{
	int i;
	int j;
	int n = g->dim;
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
		{
			printf("%i ", ((g->plateau)[n*i+j]).no_tuile);
		}
		printf("\n");
	}
	printf("\n");
}

void ajoute_tuile(int i, int j, Tuile t, Grille g)
{
	int k;
	int l;
	int n = g->dim;
	int nb = g->nb_tuiles;
	if (t->orient % 2 == 0)
	{
		for (k=i; k<i+2; k++)
		{
			for (l=j; l<j+3; l++)
			{
				((g->plateau)[k*n+l]).c = (t->cases)[(k-i)*3+(l-j)];
				((g->plateau)[k*n+l]).no_tuile = nb+1;
			}
		}
	}
	else 
	{
		for (k=i; k<i+3; k++)
		{
			for (l=j; l<j+2; l++)
			{
				((g->plateau)[k*n+l]).c = (t->cases)[(k-i)*2+(l-j)];
				((g->plateau)[k*n+l]).no_tuile = nb+1;
			}
		}
	}
	(g->tuiles)[nb] = malloc(sizeof(tuile_posee));
	((g->tuiles)[nb])->t = t;
	((g->tuiles)[nb])->i = i;
	((g->tuiles)[nb])->j = j;
	g->nb_tuiles = nb + 1;
}

int ajoute_tuile_safe(int i, int j, Tuile t, Grille g)
{
	if (!is_ingrid(t, i, j, g))
	{
		return 0;
	}
	else if(!dont_fill_lake(t, i, j, g))
	{
		return 0;
	}
	else if(!verifier_non_couvrement_total(t, i, j, g))
	{
		return 0;
	}
	else if(!is_connected(t,i,j,g))
	{
		return 0;
	}
	else
	{
		ajoute_tuile(i, j, t, g);
		return 1;
	}
}

Tuile retire_derniere_tuile(Grille g)
{
	/* nombre de tuiles posées */
	int n = g->nb_tuiles;

	if (n==0)
	{
		fprintf(stderr, "Erreur dans la fonction retire_derniere_tuile : pas de tuiles à retirer.\n");
		return NULL;
	}

	int K = g->dim;

	/*PREMIERE BOUCLE FOR*/
	
	/*i, j coordonnées du coins haut gauche de la tuile*/
	int i = g->tuiles[n-1]->i;
	int j = g->tuiles[n-1]->j;
	
	/*orientation de la tuile modulo 2 pour savoir si elle est verticale (1) ou horyzontale (0) */
	int cm = g->tuiles[n-1]->t->orient % 2;

	/*i1, j1 coordonnées du point courant DANS la tuile que l'on veut supprimer*/
	int i1, j1;

	/*a, b coordonnées de ce point dans la grille*/
	int a, b;

	/*BOUCLE WHILE*/

	/*i2, j2 coordonnées du coins haut gauche de la tuile courante du while*/
	int i2, j2;

	/*orientation de la tuile modulo 2 pour savoir si elle est verticale (1) ou horyzontale (0) */
	int cm2 = g->tuiles[n-1]->t->orient % 2;

	/*DEUXIEME BOUCLE FOR*/

	/*i3, j3 coordonnées du point courant DANS la tuile du while*/
	int i3, j3;

	/*a1, b1 coordonnées de ce point dans la grille*/
	int a1, b1;
		
	/*compteurs
	 * k1 première boucle for
	 * k2 deuxième boucle for
	 * l boucle while
	 * out : flag pour la boucle while
	 * */
	int k1, k2, l, out;
	
	/* parcours des cases de la tuile */
	for(k1=0; k1<6; k1++)
	{
		i1 = k1/(3-cm);
		j1 = k1%(3-cm);
		a = i+i1;
		b = j+j1;

		/*maintenant on va chercher quelle est la valeur de la kase du dessous*/
		/*initialisation des variables*/
		/*out = 1 => on n'a pas trouvé de terrain en dessous*/
		out = 1;
		l = n-1;
		while (l>0 && out)
		{
			l--;
			i2 = g->tuiles[l]->i;
			j2 = g->tuiles[l]->j;
			
			/*on parcours cette tuile pour voir si elle contient le point a, b*/
			for(k2=0; k2<6; k2++)
			{
				i3 = k2/(3-cm2);
				j3 = k2%(3-cm2);
				a1 = i2+i3;
				b1 = j2+j3;
				
				/*on regarde si le point correspond à celui à qui on doit trouver un remplaçant*/
				if (a1==a && b1==b)
				{
					/*on a trouvé, on tient le flag à jour*/
					out = 0;

					/*on remplace les valeurs dans plateau*/
					g->plateau[a*K+b].c = g->tuiles[l]->t->cases[k2];
					g->plateau[a*K+b].no_tuile = l+1;
				}
			}
		}
		
		/*si on n'a pas encore remplacé à la fin du while*/
		if (out)
		{
			g->plateau[a*K+b].c = N;
			g->plateau[a*K+b].no_tuile = 0;
		}
	}
	
	/* on tient la grille à jour et on revoie la tuile*/

	Tuile t = g->tuiles[n-1]->t;
	free(g->tuiles[n-1]);
	g->nb_tuiles --;

	return t;
}

void print_partie(partie p)
{
	printf("Partie :\n");
	printf("\tTaille grille : %d\n", p.tailleGrille);
	printf("\tNombre tuiles main : %d\n", p.nbTuiles);
	printf("\tIdentifiants tuiles : ");
	
	int i;
	for (i = 0; i < p.nbTuiles; i++)
		printf("%d ", p.identifiantsTuiles[i]);
	printf("\n");

	printf("\tMain du joueur : ");
	for (i = 0; i < p.nbTuiles; i++)
		printf("[Tuile id=%d] ", p.mainJoueur[i]->identifiant);
	printf("\n");

	printf("\tTuile départ : %d\n\n", p.idTuileDepart);
}

void free_partie(partie p)
{
	free(p.identifiantsTuiles);
	free(p.mainJoueur);

	p.nbTuiles = -1;
	p.mainJoueur = NULL;
	p.identifiantsTuiles = NULL;
}

partie partie_from_file(char* path, Tuile* tuiles, int nTuilesDispo)
{
	partie sortie;
	sortie.tailleGrille = -1;
	sortie.nbTuiles = -1;
	sortie.idTuileDepart = -1;
	sortie.tuileDepart = NULL;
	sortie.identifiantsTuiles = NULL;
	sortie.mainJoueur = NULL;

    FILE *file;
    file = fopen(path, "r");
    if (!file)
    {
        fprintf(stderr, "Impossible de charger la grille dans '%s'\n", path);
		return sortie;
    }

	int dim, nb_tuiles;
	fscanf(file, "%d %d\n", &dim, &nb_tuiles);

	/* 
		Si on veut une grille de taille négative ou nulle
	   	ou un nombre de tuiles négatif ou nul 
	*/
	if (dim <= 0 || nb_tuiles <= 0)
	{
		fclose(file);
		return sortie;
	}

	sortie.tailleGrille = dim;
	sortie.nbTuiles = nb_tuiles;

	int* idTuiles = malloc(nb_tuiles * sizeof(int));

	int i, lu;
	for (i = 0; i < nb_tuiles; i++)
	{
		lu = fscanf(file, "%d ", idTuiles + i);
		/* On n'a pas pu lire d'identifiant, on se plaint */
		if (lu == -1)
		{
			free(idTuiles);
			fclose(file);
			return sortie;
		}
	}

	Tuile* mainJoueur = malloc(nb_tuiles * sizeof(Tuile));

	int tuileTrouvee;
	int j;

	/* Pour chaque tuile demandée dans la partie */
	for(i = 0; i < nb_tuiles; i++)
	{
		tuileTrouvee = 0;

		/* On essaie de trouver une tuile correspondante dans les tuiles chargées */
		for(j = 0; j < nTuilesDispo; j++)
		{
			if (tuiles[j]->identifiant == idTuiles[i])
			{
				mainJoueur[i] = tuiles[j];
				tuileTrouvee = 1;
				break;
			}
		}

		/* On n'a pas trouvé de tuile chargée avec cet identifiant, on renvoie une erreur */
		if (!tuileTrouvee)
		{
			free(mainJoueur);
			free(idTuiles);
			fclose(file);
			return sortie;
			break;
		}
	}

	fscanf(file, "\n");

	int index_depart;
	lu = fscanf(file, "%d", &index_depart);
	if (lu == -1)
	{
		free(mainJoueur);
		free(idTuiles);
		fclose(file);
		return sortie;
	}

	/* Définition du champ tuileDepart (pointeur vers la tuile de départ) */
	tuileTrouvee = 0;
	if (index_depart >= nb_tuiles)
	{
		free(mainJoueur);
		free(idTuiles);
		fclose(file);
		return sortie;
	}

	sortie.identifiantsTuiles = idTuiles;
	sortie.mainJoueur = mainJoueur;
	sortie.idTuileDepart = idTuiles[index_depart];
	sortie.tuileDepart = tuiles[sortie.idTuileDepart];

	fclose(file);
    return sortie;
}

int grille_to_file(char* path, Grille g, Tuile* main, int n, int tuileDepart)
{
    FILE *file;
    file = fopen(path, "w");
    if (!file)
    {
		printf("Impossible d'exporter la grille vers '%s'\n", path);
		return 0;
	}
	
	fprintf(file, "%d %d\n", g->dim, n);
	
	int i;
	for (i = 0; i < n; i++)
	{
		if (i < n - 1)
			fprintf(file, "%d", main[i]->identifiant);
		else
			fprintf(file, "%d\n", main[i]->identifiant);
	}

	fprintf(file, "%d", tuileDepart);
    fclose(file);

	return 1;
}

int coord_to_brut(int i, int j, Grille g)
{
	return i * g->dim + j;
}

int not_in_list(int* l, int n, int x)
{
	int i;
	for (i = 0; i < n; i++)
		if (l[i] == x)
			return 0;

	return 1;
}

int village_associe(int* village, int i, int j, Grille g)
{
	int k;
	for (k = 0; k < g->dim * g->dim; k++)
		village[k] = -1;

	/* Si la case de départ n'est pas un village */
	if (lecture_case(i, j, g) != V)
		return 0;

	int casesGrille = g->dim * g->dim;
	int* aExplorer = malloc(casesGrille * sizeof(int));

	for (k = 0; k < casesGrille; k++)
		aExplorer[k] = -1;
		
	k = 0;

	/* On remplit la première case de la liste des cases à explorer */
	aExplorer[0] = i * g->dim + j;
	
	/* La dernière case du tableau aExplorer remplie */
	int indice = 0;
	int col, row;

	/* Et on explore ! */
	while (aExplorer[k] != -1)
	{
		/*
		printf("k = %d, i = %d, j = %d, case = %c\n", 
				k,
				aExplorer[k] / g->dim,
				aExplorer[k] % g->dim,
				lecture_case_brut(aExplorer[k], g));
		*/

		col = aExplorer[k] % g->dim;
		row = aExplorer[k] / g->dim;
		
		village[k] = aExplorer[k];

		if (row > 0)
			if (lecture_case(row - 1, col, g) == V)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row - 1, col, g)))
					aExplorer[++indice] = coord_to_brut(row - 1, col, g);

		if (row < g->dim - 1)
			if (lecture_case(row + 1, col, g) == V)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row + 1, col, g)))
					aExplorer[++indice] = coord_to_brut(row + 1, col, g);

		if (col > 0)
			if (lecture_case(row, col - 1, g) == V)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row, col - 1, g)))
					aExplorer[++indice] = coord_to_brut(row, col - 1, g);

		if (col < g->dim - 1)
			if (lecture_case(row, col + 1, g) == V)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row, col + 1, g)))
					aExplorer[++indice] = coord_to_brut(row, col + 1, g);

		k++;
	}

	free(aExplorer);
	return indice + 1;
}

int* cases_recouvertes(int i, int j, Tuile t, Grille g)
{
	/* Si on veut placer la tuile en dehors de la grille */
	if (!is_ingrid(t, i, j, g))
		return NULL;

	/* Une tuile recouvre au maximum 6 cases */
	int* cases = malloc(6 * sizeof(int));
	int k;
	for (k = 0; k < 6; k++)
		cases[k] = -1;

	int n = g->dim;

	/* Représente l'indice actuel dans le tableau que l'on remplit */
	int indice = 0;

	/* Le tableau des coordonnées à explorer */
	int coordonnees[6];

	/* Cas tuile verticale */
	if (t->orient % 2 == 0)
	{
		coordonnees[0] = i * n + j;
		coordonnees[1] = i * n + j + 1;
		coordonnees[2] = i * n + j + 2;
		coordonnees[3] = (i + 1) * n + j;
		coordonnees[4] = (i + 1) * n + j + 1;
		coordonnees[5] = (i + 1) * n + j + 2;

		for(k = 0; k < 6; k++)
		{
			if (lecture_case_brut(coordonnees[k], g) != N)
				cases[indice++] = coordonnees[k];
		}
	}
	else
	{
		coordonnees[0] = i * n + j;
		coordonnees[1] = i * n + j + 1;
		coordonnees[2] = (i + 1) * n + j;
		coordonnees[3] = (i + 1) * n + j + 1;
		coordonnees[4] = (i + 2) * n + j;
		coordonnees[5] = (i + 2) * n + j + 1;

		for(k = 0; k < 6; k++)
		{
			if (lecture_case_brut(coordonnees[k], g) != N)
				cases[indice++] = coordonnees[k];
		}	
	}
	return cases;
}

int* terrains_recouverts(Grille g)
{
	int n = g-> dim;
	int k;
	int * t = malloc(n*n*sizeof(int));
	for (k=0; k<n*n; k++)
	{
		if (g->plateau[k].c == N)
		{	
			t[k] = 0;
		}
		else 
		{
			t[k] = 1;
		}
	}
	return t;
}
