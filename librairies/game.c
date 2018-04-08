#include "game.h"
#include "grille.h" /*besoin de coord_to_brut et not_in_list pour le calcul des scores*/

void dessiner_grille()
{
    int k;
	int k1;
	int N = 20;
	printf("\n");
	for (k = 0; k < N; k++)
	{
		if (k == 0 || k==N-1)
		{	
			printf("+");
			if (k == N)
				printf("\n");
		}
		else 
			printf("--");
	}
	printf("\n");
	for (k1 = 0; k1 < N - 2; k1++)
	{
		for (k = 0; k < N; k++)
		{
			if (k == 0 || k==N-1)
				printf("|");
			else 
				printf("  ");
		}
		printf("\n");
	}
	for (k = 0; k < N; k++)
	{
		if (k == 0 || k==N-1)
			printf("+");
		else 
			printf("--");
	}
	printf("\n");
}

int main(void)
{
    /* Ajout d'un handler pour le signal de redimensionnement de fenêtre */
    /* signal(SIGWINCH, handle_winch); */

    /* Récupération de la taille du terminal */
    /* initscr(); */

    clear_term();
    
    int largeur = 50;
    int hauteur = 20;
    int largeurMenu = 22;
    int hauteurTuiles = 5;

    int nTuiles;
    Tuile* tuiles = tuiles_from_file("fichiers/tuiles", &nTuiles);
    partie p = partie_from_file("fichiers/Partie1", tuiles, nTuiles);

    int score = game(largeur, hauteur, largeurMenu, hauteurTuiles, p);

    free(p.mainJoueur);
    free_tableau(tuiles, nTuiles);
    gotoxy(hauteur + 1, 0);
    printf("Le jeu est fini. Score total : %d\n", score);
    getchar();
    clear_term();

	return 0;
}

typedef enum touche touche;

int getch()
{
    struct termios oldtc;
    struct termios newtc;
    int ch;
    tcgetattr(STDIN_FILENO, &oldtc);
    newtc = oldtc;
    newtc.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
    return ch;
}


int game(int largeur, int hauteur, int largeurMenu, int hauteurTuiles, partie p)
{
    dessiner_cadre(largeur, hauteur, largeurMenu, hauteurTuiles);

    char input;
    int id;

    GameState state = init_game_state(largeur, hauteur, largeurMenu, hauteurTuiles, p);

    dessiner_cadre(largeur, hauteur, largeurMenu, hauteurTuiles);
    updateGrille(state);
    updateTuileAPlacer(state);
    updateTuileEnCours(state);
    print_score(state, 0, 0);
	gotoxy(3,0);
	printf("Commandes :");
	gotoxy(5,0);
	printf("1) a, z :\n  Choisir la tuile");
	gotoxy(7,0);
	printf("2) Flèches :\n  Bouger le curseur");
	gotoxy(9,0);
	printf("3) r : \n  Orienter la tuile");
	gotoxy(11,0);
	printf("4) p : \n  Poser la tuile");

    int stop = 0;
    while (!stop)
    {
        updateTuileAPlacer(state);
        input = getch();

        switch(input)
        {
            case Haut:
                if (state->iNouvelleTuile == 0) break;

                state->iNouvelleTuile--;
                updateGrille(state);
                break;

            case Droite:
                if (state->jNouvelleTuile == state->g->dim - 1) break;

                state->jNouvelleTuile++;
                updateGrille(state);
                break;

            case Bas:
                if (state->iNouvelleTuile == state->g->dim - 1) break;

                state->iNouvelleTuile++;
                updateGrille(state);
                break;

            case Gauche:
                if (state->jNouvelleTuile == 0) break;

                state->jNouvelleTuile--;
                updateGrille(state);
                break;

            case Rotation:
                plus_pi_4(state->tuileSelectionnee);
                updateGrille(state);
                updateTuileEnCours(state);
                break;

            case Placement:
                if (ajoute_tuile_safe(state->iNouvelleTuile, state->jNouvelleTuile, state->tuileSelectionnee, state->g))
                {
                    int k;
                    for (k = 0; k < state->tuilesDisponibles; k++)
                        if (state->tuiles[k] == state->tuileSelectionnee)
                        {
                            state->tuiles[k] = NULL;
                            break;
                        }

                    /* On affiche la grille avec la tuile posée */
                    updateGrille(state);
                    calculer_score(state);
                    print_score(state,0,0);
                     gotoxy(hauteur + 1, 0);
                    
                    /* On choisit une nouvelle tuile */
                    state->tuileSelectionnee = trouver_premiere_tuile(state);
                    state->indiceTuileSelectionnee = trouver_indice_premiere_tuile(state);

                        /* Si on n'a pas trouvé de nouvelle tuile dans la main, on sort du programme */
                        if (state->tuileSelectionnee == NULL)
                    {
                        stop = 1;
                        break;
                    }

                    updateTuileEnCours(state);
                }
                else{
                    gotoxy(hauteur - 1, 0);
                    printf("Impossible de placer la tuile en (%d, %d)\n", state->iNouvelleTuile, state->jNouvelleTuile);
                }
                break;
            case Precedent:
                id = state->indiceTuileSelectionnee;
                id--;
                while (id > 0 && state-> tuiles[id] == NULL)
                {
                    id--;
                }
                if(id>=1){
                    state->indiceTuileSelectionnee = id;
                    state->tuileSelectionnee = state->tuiles[id];
                }else{
                    /*Hors Tableau*/
                }
                updateTuileEnCours(state);
                break;

            case Suivant:
                id = state->indiceTuileSelectionnee;
                id++;
                while (id < 11 && state->tuiles[id] == NULL)
                {
                    id++;
                }
                if (id <= 11)
                {
                    state->indiceTuileSelectionnee = id;
                    state->tuileSelectionnee = state->tuiles[id];
                }else{
                    /*Hors Tableau*/
                }
                updateTuileEnCours(state);
                break;
            /* Premières "frappes" lors des appuis sur les touches directionnelles */
            case 27: case 91:
                break;
            case -1: /* jsaipalol */
                break;

            /**
             *  10 = Entrée
             */
            case 10: 
                break;

            /* Si on trouve une touche qu'on connaît pas */
            default:
                if (0)
                    printf("Touche inconnue : %d\n", input);
                
            
            
        }
        printf(KWHT);
        trouverVillageMax(state);
        print_score(state, 0, 0);
        gotoxy(hauteur + 1, 0);
    }
    
    state->score = calculer_score(state);
    int score = state->score;
    print_score(state,0,2);
    free(state);
    return score;
}

Tuile trouver_premiere_tuile(GameState state)
{
    int i;
    for (i = 0; i < state->tuilesDisponibles; i++)
        if (state->tuiles[i] != NULL)
            return state->tuiles[i];

    return NULL;
}

void liberer_state(GameState state)
{
    free_grille(state->g);
    
    /*
    free_tableau(state->tuiles, state->tuilesDisponibles);
    */

    free(state->villageMax);
    free(state);
}

int trouver_indice_premiere_tuile(GameState state)
{
    int i;
    for (i = 0; i < state->tuilesDisponibles; i++)
        if (state->tuiles[i] != NULL)
            return i;

    return -1;
}

GameState init_game_state(int largeur, int hauteur, int largeurMenu, int hauteurTuiles, partie p)
{
    GameState state = malloc(sizeof(gameState));
    state->tuilesDisponibles = p.nbTuiles;
    state->tuiles = p.mainJoueur;
    state->indiceTuileSelectionnee=1;

    state->iNouvelleTuile = 0;
    state->jNouvelleTuile = 0;
    state->largeur = largeur;
    state->hauteur = hauteur;
    state->largeurMenu = largeurMenu;
    state->hauteurTuiles = hauteurTuiles;
    state->tailleVillageMax = 0;
    state->villageMax = malloc(p.tailleGrille * p.tailleGrille * sizeof(int));
    state->score = 0;


    /* On construit la grille de la partie */
    state->g = nouvelle_grille(p.tailleGrille);
    ajoute_tuile(state->g->dim / 2 - 1, state->g->dim / 2 - 1, p.tuileDepart, state->g);
    
    int k;
    for (k = 0; k < state->tuilesDisponibles; k++)
    {
        if (state->tuiles[k] == p.tuileDepart)
        {
            state->tuiles[k] = NULL;
            break;
        }
    }

    /* Libération des identifiants des tuiles dans partie */
    free(p.identifiantsTuiles);

    state->tuileSelectionnee = trouver_premiere_tuile(state);

    state->score = calculer_score(state);
    trouverVillageMax(state);

    return state;
}

void trouverVillageMax(GameState state)
{
    int i, j, k;
    int kMax = 0;
    int tailleMaxVillage = 0;
    int tailleVillage;
    int* tempVillage = malloc(state->g->dim * state->g->dim * sizeof(int));
    for (k = 0; k < state->g->dim * state->g->dim; k++)
    {
        i = k / state->g->dim;
        j = k % state->g->dim;

        tailleVillage = village_associe(tempVillage, i, j, state->g);

        if (tailleVillage > tailleMaxVillage)
        {
            kMax = k;
            tailleMaxVillage = tailleVillage;
        }
    }
    
    state->tailleVillageMax = village_associe(state->villageMax, kMax / state->g->dim, kMax % state->g->dim, state->g);
    free(tempVillage);
}

int min(int a,int b){
    if (a<=b) return a;
    else return b;
}
int max(int a,int b){
    if (a>=b) return a;
    else return b;
}
int calculer_score(GameState state)
{
    int score = 0;
    Grille gril = state->g;
    int forets = 0;
    int Lacs = 0;
    int ressources = 0;
    int usine = 0;
    int maxVillage = 0;
    int* Lacvus = malloc((gril->dim*gril->dim)*sizeof(int));
    int* VillageVus = malloc((gril->dim*gril->dim)*sizeof(int));
    int i,j;
    for (i = 0; i < gril->dim * gril->dim; i++)
		Lacvus[i] = 0;
        VillageVus[i] = 0;
    for(i = 0;i < gril->dim;i++){
        for(j=0;j < gril->dim;j++){
            kase ca = gril->plateau[i*gril->dim+j].c;
            switch(ca){
                case V:
                    maxVillage = max(maxVillage,calcul_Village(VillageVus,i,j,gril));
                    break;
                case F:
                    forets+=1;
                    break;
                case L:
                    if(Lacvus[i*gril->dim+j]==0) Lacs+=calcul_Lac(Lacvus,i,j,gril)-1;
                    /*gotoxy(5, 0);	DEBUG
                    printf("Nb de lacs %d",Lacs);*/
                    break;
                case U:
                    usine+=1;
                    break;
                case R:
                    ressources+=1;
                    break;
                default:
                    break;
            }
        }
    }
    free(Lacvus);
    free(VillageVus);
	/*debug
    gotoxy(2,0);
    printf("MaxVillage %d",maxVillage);
    gotoxy(3,0);
    printf("Forêt %d",forets);*/
    score+= (Lacs)*3 + forets*2 + min(usine,ressources)*4+maxVillage;
    state->score = score;
    gotoxy(0, 0);
    /*printf("%p", &state);*/
    return score;
}

void print_score(GameState state,int x,int y){
    gotoxy(x,y);
    printf("             ");
    gotoxy(x,y);
    printf("Score : %d",state->score);
}

int calcul_Lac(int* Lacs, int i, int j, Grille g)
{
	int k;
	/*for (k = 0; k < g->dim * g->dim; k++)
		Lacs[k] = -1;*/

	/* Si la case de départ n'est pas un village */
	if (lecture_case(i, j, g) != L)
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
		
		Lacs[aExplorer[k]] = 1;

		if (row > 0)
			if (lecture_case(row - 1, col, g) == L)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row - 1, col, g)))
					aExplorer[++indice] = coord_to_brut(row - 1, col, g);

		if (row < g->dim - 1)
			if (lecture_case(row + 1, col, g) == L)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row + 1, col, g)))
					aExplorer[++indice] = coord_to_brut(row + 1, col, g);

		if (col > 0)
			if (lecture_case(row, col - 1, g) == L)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row, col - 1, g)))
					aExplorer[++indice] = coord_to_brut(row, col - 1, g);

		if (col < g->dim - 1)
			if (lecture_case(row, col + 1, g) == L)
				if (not_in_list(aExplorer, casesGrille, coord_to_brut(row, col + 1, g)))
					aExplorer[++indice] = coord_to_brut(row, col + 1, g);

		k++;
	}

	free(aExplorer);
	return indice + 1;
}

int calcul_Village(int* Village, int i, int j, Grille g)
{
	int k;
	/*for (k = 0; k < g->dim * g->dim; k++)
		Lacs[k] = -1;*/

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
		
		Village[aExplorer[k]] = 1;

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

void update(GameState state)
{
    updateGrille(state);
    updateTuileEnCours(state);
}

void updateGrille(GameState state)
{
    affiche_plateau_2_jeu(state->tuileSelectionnee, state->g, 1, state->largeurMenu + 1, state->iNouvelleTuile, state->jNouvelleTuile);
}

void updateTuileAPlacer(GameState state)
{
    affiche_tuile_dispo(state->hauteur, state->tuiles, state->indiceTuileSelectionnee);
}

void updateTuileEnCours(GameState state)
{
    affiche_plateau_2_jeu(state->tuileSelectionnee, state->g, 1, state->largeurMenu + 1, state->iNouvelleTuile, state->jNouvelleTuile);
}

void affiche_plateau_2_jeu(Tuile t, Grille g, int x, int y, int i, int j)
{
	int n = g->dim;
	
	int c = t->orient % 2;

	/* printf("c = %i\n", c); */

	int pose_possible = test_tout(t, i, j, g);

	/*compteurs*/
	int k;
	int k1;

	/*le + 4 est là pour prendre en compte le déclage lié à lindexation des cases*/
	gotoxy(x,y+2);
	printf(KWHT "| ");
	for (k=0; k<n; k++)
	{
		printf(KWHT "%i ", k);
	}
	x++;
	gotoxy(x, y);
	printf(KWHT "---");

	for (k=0;k<n;k++)
	{	
		printf(KWHT "--");
	}
	x++;
	for (k=0; k<n; k++)
	{
		gotoxy(x,y);
		printf(KWHT "%i |", k);
		
		for (k1=0; k1<n; k1++)
		{
			/* on est sur la tuile */
			if (k >= i && 
				k <= i + 1 + c &&
				k1 >= j &&
				k1 <= j + 2 - c)
			{
				if (pose_possible)
				{
					printf(KCYN "%c ", t->cases[(k-i) * (3-c) + (k1-j)]);
				}
				else 
				{
					printf(KRED "%c ", t->cases[(k-i) * (3-c) + (k1-j)]);
				}
			}
			else 
			{
				kase terrain = g->plateau[n*k+k1].c; 
				switch (terrain)
				{
					case P :
						printf(KYEL "%c ", terrain);
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
						printf(KMAG "%c ", terrain);
						break;
					case U :
						printf(KMAG "%c ", terrain);
						break;
					case N :
						printf("%c ", terrain);
						break;
				}
			}
		}
		x++;
	}
}

void affiche_tuile_dispo(int hauteur, Tuile* tuiles, int indiceTuileSelectionnee)
{
    /*compteurs*/
    int k;
    int k1;
    int i=0;
    int x;
    int y;
    int largeurTuile = 8;
    gotoxy(hauteur - 5, 5);

    for (k = 1; k < 12; k++)
    {
        x = hauteur - 5;
        y = i * largeurTuile;
        gotoxy(x, y);       
        if(tuiles[k] != NULL){
            for (k1 = 0; k1 < 6; k1++)
            {
                if(k1%2==0){
                    x++;
                    gotoxy(x,y);
                }
                kase terrain = tuiles[k]->cases[k1];
                if (k == indiceTuileSelectionnee)
                {
                    printf(KCYN "%c ", terrain);
                }
                else
                {
                    switch (terrain)
                    {
                    case P:
                        printf(KYEL "%c ", terrain);
                        break;
                    case F:
                        printf(KGRN "%c ", terrain);
                        break;
                    case L:
                        printf(KBLU "%c ", terrain);
                        break;
                    case V:
                        printf(KWHT "%c ", terrain);
                        break;
                    case R:
                        printf(KMAG "%c ", terrain);
                        break;
                    case U:
                        printf(KMAG "%c ", terrain);
                        break;
                    case N:
                        printf("%c ", terrain);
                        break;
                    }
                }
                
            }
            i++;
        }
    }
    int k2;
    x = hauteur - 5;
    y = i * largeurTuile;
    
    for(k2=1;k2<4;k2++){
        gotoxy(x+k2, y);
        printf("    ");
    }
}

#define CHR_COL "|"
#define CHR_ROW "-"

void dessiner_cadre(int largeur, int hauteur, int largeurMenu, int hauteurTuiles)
{
    clear_term();

    gotoxy(0, 0);
    int i, j;
    for (i = 0; i < hauteur - hauteurTuiles - 1; i++)
    {
        gotoxy(i, largeurMenu);
        printf(CHR_COL);
    }

    for (j = 0; j < largeur; j++)
    {
        gotoxy(hauteur - (hauteurTuiles + 1), j);
        printf(CHR_ROW);
    }
    gotoxy(hauteur + 1, 0);
}
