/**
 * \file ytuile.c
 * Ce fichier implémente les fonctions de \a ytuile.h.
 */

#include "tuile.h"

void print_tuile(Tuile t)
{
	int i;
	if (t->orient % 2 == 0)
	{
		for (i=0; i<6; i++)
		{
			printf("%c ", t->cases[i]);
			if (i % 3 == 2)
			{
				printf("\n");
			}
		}
	}
	else 
	{
		for (i=0; i<6; i++)
		{
			printf("%c ", t->cases[i]);
			if (i % 2 == 1)
			{
				printf("\n");
			}
		}
	}
}

void echange(int i, int j, kase cases[6])
{
	kase temp = cases[i];
	cases[i] = cases[j];
	cases[j] = temp;
}

void plus_pi_4(Tuile t)
{
	if(t->orient % 2 == 0)
	{
		/*cas nord->ouest ou sud->est*/
		echange(2,0,t->cases);
		echange(5,1,t->cases);
		echange(5,2,t->cases);
		echange(4,3,t->cases);
		echange(5,4,t->cases);
	}
	else 
	{
		/*cas est->nord ou ou ouest->sud*/
		echange(0,1,t->cases);
		echange(3,1,t->cases);
		echange(5,2,t->cases);
		echange(4,5,t->cases);
	}
	t->orient = (t->orient+1) % 4;
}

void oriente(Tuile t, card c)
{
	int j = (c - t->orient + 4) % 4;
	int i;
	for(i=0; i<j; i++)
	{
		plus_pi_4(t);
	}
}

Tuile rand_tuile(void)
{
	srand(time(NULL));
	Tuile r = malloc(sizeof(tuile));
	int i;
	int x;
	char cases[6] = {'P', 'F', 'L', 'V', 'R', 'U'};

	for(i=0; i<6; i++)
	{
		x = rand() % 6;
		r->cases[i] = (kase)cases[x];
	}

	r->orient = O_E;

	/*
	x = rand() % 4;
	r->orient = x;
	*/
	r->fromFile = 0;
	return r;
}

Tuile* tuiles_from_file(char* path, int* tuilesImportees)
{
	if (tuilesImportees != NULL)
		*tuilesImportees = -1;

    FILE *file;
    file = fopen(path, "r");
    if (!file)
    {
        fprintf(stderr, "Impossible de charger la tuile dans '%s'\n", path);
		return NULL;
    }

	int n;
	if (fscanf(file, "%d\n", &n) == -1)
	{
		fclose(file);
		return NULL;
	}

	Tuile* tuiles = malloc(n * sizeof(Tuile));

	/* Variables temporaires utilisées pour lire le contenu du fichier */
	char a, b, c, d, e, f;
	/* Variables d'itération */
	int i, j;
	/* Variables pour stocker le nombre d'octets lus à différents endroits */
	int lu, lu1, lu2, lu3;
	/* A t-on échoué ? */
	int failed = 0;
	for (i = 0; i < n; i++)
	{
		a = 0; b = 0; c = 0; d = 0; e = 0; f = 0;

		/* On initialise et stocke notre tuile */
		tuiles[i] = malloc(sizeof(tuile));	

		/* On lit l'ID de la tuile */
		int id;
		lu = fscanf(file, "%d\n", &id);
		if (lu == -1)
		{
			failed = 1;
			break;
		}
		tuiles[i]->identifiant = id;
		tuiles[i]->fromFile = 1;

		/* 
		 * Les tuiles sont fournies "à la verticale"
		 * c'est à dire en orientation "Est" pour nous
		 */
		tuiles[i]->orient = O_E;

		/* 
		 * On lit ses cases depuis le fichier
		 * Je suis pas particulièrement fier de ça, mais ça ne 
		 * fonctionnait pas de les mettre directement dans la
		 * structure
		*/
		lu1 = fscanf(file, "%c %c\n", &a, &b);
		lu2 = fscanf(file, "%c %c\n", &c, &d);
		lu3 = fscanf(file, "%c %c\n", &e, &f);

		/* Si une des lectures a échoué (pas de caractères à lire) */
		if (lu1 == -1 || lu2 == -1 || lu3 == -1)
		{
			failed = 1;
			break;
		}

		/* Si une des lectures n'a pas été correcte au niveau du format */
		if (a == 0 || b == 0 || c == 0 ||
			d == 0 || e == 0 || f == 0)
		{
			failed = 1;
			break;
		}

		/* Remplissage des cases */
		(tuiles[i]->cases)[0] = (kase)a;
		(tuiles[i]->cases)[1] = (kase)b;
		(tuiles[i]->cases)[2] = (kase)c;
		(tuiles[i]->cases)[3] = (kase)d;
		(tuiles[i]->cases)[4] = (kase)e;
		(tuiles[i]->cases)[5] = (kase)f;

		/* Vérification que toutes les cases sont bien des terrains connus */
		int terrainInconnu = 0;
		for(j = 0; j < 6; j++)
			if (tuiles[i]->cases[j] != V && 
				tuiles[i]->cases[j] != P && 
				tuiles[i]->cases[j] != F && 
				tuiles[i]->cases[j] != L && 
				tuiles[i]->cases[j] != R &&
				tuiles[i]->cases[j] != U)
			{
				terrainInconnu = 1;
				break;
			} 
		if (terrainInconnu)
		{
			failed = 1;
			break;
		}
	}
	/* On ferme le fichier */
	fclose(file);

	/**
	 * S'il y a eu un problème lors du chargement des tuiles
	 * On free tout ce qu'on a alloué et on renvoyé NULL
	 */
	if (failed)
	{
		for (j = 0; j <= i; j++)
			free(tuiles[j]);
		free(tuiles);
		return NULL;
	}

	if (tuilesImportees != NULL)
		*tuilesImportees = n;

	return tuiles;
}

void exporter_tuiles(char* path, Tuile* tuiles, int n)
{   
	FILE *file;
    file = fopen(path, "w");
    if (!file)
	{
		printf("Impossible d'exporter.\n");
		_exit(1);
	}

	fprintf(file, "%d\n", n);

	int i;
	Tuile t;
	for (i = 0; i < n; i++)
	{
		t = tuiles[i];

		fprintf(file, "%d\n", t->identifiant);
		fprintf(file, "%c %c\n", t->cases[0], t->cases[1]);
		fprintf(file, "%c %c\n", t->cases[2], t->cases[3]);
		fprintf(file, "%c %c\n", t->cases[4], t->cases[5]);
	}

    fclose(file);
}

void free_tableau(Tuile* tab, int n)
{
	int i;
	for (i = 0; i < n; i++)
		if (tab[i]->fromFile)
			free(tab[i]);
	free(tab);
}