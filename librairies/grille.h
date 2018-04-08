/*! \mainpage Honshu
 *
 * \section intro_sec Introduction
 *
 * Une version numérique du jeu de plateau \b Honshu.
 * Dans la version actuelle, il n'y a que le lot A : des fonctions de base pour charger des parties, des tuiles, gérer une partie, etc.
 *
 * \section groupe Présentation du groupe
 * 
 * - Thomas KOWALSKI
 * - Thibaut MILHAUD
 * - Matthis PILLONEL
 * - Yoan ROCK (FIPA)
 * 
 * \section install_sec Compilation
 *
 * \subsection testsU Tests unitaires (A)
 *  
 * make u_a && ./unit_A
 * 
 * \subsection valgrind_A valgrind (A)
 * 
 * make vg_a
 */

/**
 * \file grille.h
 * Ce fichier contient la description des types :
 * - \b tuille_posee;
 * - \b Tuile_p, pointeur vers le type tuile_posee;
 * - \b grille;
 * - \b Grille, pointeur vers el type grille.
 *
 * Ainsi que les fonctions de base traitant de ces structures
 */

#ifndef YGRILLE_H
#define YGRILLE_H

#include "tuile.h"



/**
 * \struct partie
 * \brief Représente une partie chargée depuis un fichier.
 */

struct partie {
	int tailleGrille; /*!<La taille de la grille de la partie*/
	int nbTuiles; /*!<Le nombre de tuiles dans la main du joueur*/
	int* identifiantsTuiles; /*!<Les identifiants des tuiles (pour les retrouver depuis un fichier de tuiles) */
	int idTuileDepart; /*!<L'identifiant de la tuile posée au début sur le plateau*/
	Tuile tuileDepart; /*!<La tuile de départ sur la grille */
	Tuile* mainJoueur; /*!<Les tuiles que le joueur a dans sa main.*/
};


typedef struct partie partie;

typedef partie* Partie;

/**
 * \brief Affiche une description d'une partie
 * \param p la partie à afficher
 */
void print_partie(partie p);

/**
 * \brief Libère l'espace mémoire d'une partie
 * \param p la partie à libéré
 * \warning L'espace mémoire utilisé par les Tuiles de la main n'est pas libéré !
 * Il faut le faire avec \b free_tableau !
 */
void free_partie(partie p);

/**
 * \struct kase_n
 * \brief Représente une case dans une tuile
 */


struct kase_n{
	kase c; /*!<Le terrain de cette case*/
	int no_tuile; /*!<L'identifiant de la tuile à laquelle appartient cette case.*/
};

typedef struct kase_n kase_n;


/**
 * \struct tuile_posee
 * \brief Représente une tuile posée sur le plateau
 */

struct tuile_posee{
	Tuile t; /*!<La tuile (non posée) qui est posée*/
	int i, j; /*!<Les coordonnées de la case en haut a gauche de la Tuile sur la Grille*/
};

typedef struct tuile_posee tuile_posee;

/**
 * \typedef Tuile_p
 * \brief Pointeur vers une tuile_posee
 */

typedef tuile_posee* Tuile_p;

/**
 * \struct grille
 * \brief Représente une grille de jeu Honshu.
 */

struct grille{
	kase_n* plateau; /*!<Le plateau de jeu, un tableau de \b enum \b case*/
	int nb_tuiles; /*!<Le nombre de tuiles posées sur le plateau*/
	int dim; /*!<Le côté du plateau de jeu*/
	Tuile_p tuiles[13]; /*!<Toutes les tuiles qui ont été posées depuis le début du jeu*/
};

typedef struct grille grille;

/**
 * \typedef Grille
 * \brief Pointeur vers une grille
 */

typedef grille* Grille;

/**
 * \brief Creé une Grille de \b n cases de cotés initialisées à vide
 * \param n un entier
 * \return Une Grille vide
 */
Grille nouvelle_grille(int n);

/**
 * \author Thomas Kowalski
 * \brief Creé une Grille de \b n cases de cotés initialisées à vide avec une tuile aléatoire au milieu.
 * \param n un entier
 * \return Une Grille avec une tuile aléatoire.
 */
Grille nouvelle_grille_avec_tuile(int n);


/**
 * \author Thomas Kowalski
 * \brief Fonction qui libère l'espace mémoire utilisé par une Grille
 * \param g La Grille en question 
 */

void free_grille(Grille g);

/**
 * \brief Renvoie la valeur de la case de coordonnées (\b i, \b j) dans la grille \b g.
 * Renvoie 0 si les coordonnées sont en dehors de la grille.
 * \param i abscisse discrete
 * \param j ordonnée discrete
 * \param g Grille
 * \return la valeur de la case déterminée par les coordonnées
 */

kase lecture_case(int i, int j, Grille g);

/**
 * \brief Renvoie la valeur de la case de coordonnées (\b i, \b j) dans la grille \b g
 * Renvoie 0 si la coordonnée est en dehors de la grille.
 * \param i abscisse discrete
 * \param j ordonnée discrete
 * \param g Grille
 * \return la valeur de la case déterminée par les coordonnées
 */
kase lecture_case_brut(int k, Grille g);

/**
 * \brief Affiche la Grille dans l'état actuel
 * \param g Grille à afficher
 */

void print_grille(Grille g);

/**
 * \brief Affiche le numero de la tuile occupant chaque case;
 * \param g la Grille à afficher
 */

void print_politique(Grille g);

/**
 * \brief Ajoute la Tuile \b t dans la Grille \b g en mettant le coin haut/gauche en (\b i, \b j)
 * \param i abscisse du point
 * \param j odonnée du point
 * \param t la Tuile à ajouter
 * \param g la Grille où l'on ajoute la tuile
 * \attention Pas de garde fous cette fonction permet de réaliser des actions illégales comme recouvrir totalement une tuile déjà posée;
 */

void ajoute_tuile(int i, int j, Tuile t, Grille g);

/**
 * \brief Ajoute la Tuile en verifiant :
 * - qu'on ne recouvre pas un lac,
 * - qu'on ne recouvre pas totalement une autre tuile,
 * - qu'il est dans la grille \b t dans la Grille \b g en mettant le coin haut/gauche en (\b i, \b j)
 * \param i abscisse du point
 * \param j odonnée du point
 * \param t la Tuile à ajouter
 * \param g la Grille où l'on ajoute la tuile
 * \return \b 1 si la tuile a été ajoutée, \b 0 sinon
 */

int ajoute_tuile_safe(int i, int j, Tuile t, Grille g);

/**
 * \author Thibaud Milhaud
 * \brief Retire la derière Tuile posée dans la Grille \b g 
 * \param g la Grille où l'on enlève la tuile
 * \return la Tuile retirée
 */

Tuile retire_derniere_tuile(Grille g);

/**
 * \author Thibaud Milhaud
 * \brief Charge la grille depuis le fichier
 * \param path le fichier à charger
 * \param tuiles les tuiles disponibles (chargées depuis un fichier)
 * \param nTuilesDispo le nombre de tuiles disponibles
 * \return La partie chargée depuis le fichier \b path
 * \warning En cas d'erreur, la partie renvoyée a ses champs définis avec des valeurs par défaut (\b -1 pour les entiers, \b NULL pour les tableaux)
 */
partie partie_from_file(char* path, Tuile* tuiles, int nTuilesDispo);

/**
 * \brief Exporte \b g vers \b path.
 * \author Thomas Kowalski
 * \param path le fichier où il faut exporter
 * \param g la grille à exporter
 * \param main la main du joueur (un tableau de \b struct \b tuile)
 * \param n la taille de \b main
 * \param tuileDepart l'identifiant de la tuile posée au départ
 * \return 0 si l'exportation a échoué, 1 sinon
 */
int grille_to_file(char* path, Grille g, Tuile* main, int n, int tuileDepart);

/**
 * \brief Renvoie le (plus grand) village connexe.
 * \author Thomas Kowalski
 * \param village un tableau d'entiers représentant les cases du tableau associées au village (\b à \b free)
 * \param i la ligne de la case Ville (>= 0)
 * \param j la colonne de la case Ville (>= 0)
 * \param g la grille de jeu
 * \return Le nombre d'éléments du tableau
 */
int village_associe(int* village, int i, int j, Grille g);
int coord_to_brut(int i, int j, Grille g);
int not_in_list(int* l, int n, int x);
/**
 * \author Thomas Kowalski
 * \brief Renvoie la liste des cases recouvertes par une tuile si on la pose sur la grille
 * \param i la coordonnée verticale
 * \param j la coordonnée horizontale
 * \param t la tuile à poser
 * \param g la grille sur laquelle poser la tuile
 * \return un tableau d'indices de tableau
 * \warning La fonction renvoie une liste dont il faut ensuite libérer la mémoire
 * \warning La fonction renvoie les cases recouvertes sous la forme d'indices dans le tableau de la grille, pas sous forme de coordonnées
 */
int* cases_recouvertes(int i, int j, Tuile t, Grille g);

/**
 * \brief Renvoie un tableau dont chaque case contient 1 si la case correspondante de la grille est recouverte et 0 sinon.
 * \author Thibaut Milhaud
 * \param g la grille que l'on étudie
 * \return un tableau de 0 et de 1
 * \warning La fonction renvoie un tableau alloué par \b malloc, qu'il faut libérer, à terme.
 */
int* terrains_recouverts(Grille g);

#endif