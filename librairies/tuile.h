/**
* \file tuile.h
* Ce fichier contient la description des types :
* - \b kase, énumération des types de cases (plaine, forêt, lac, village, ressource, usine, vide.);
* - \b card, énumération des orientations possibles (N, O, S, E);
* - \b tuile, union d'un tableau de 6 cases et une orientation;
* - \b Tuile, pointeur vers le type tuile;
*
* Ansi que des fonction de base pour traiter ces types.
*/

#if !defined YTUILE_H
#define YTUILE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
 * \enum kase
 * \brief Liste les diférents types possibles pour une case
 * \attention Pour les afficher il faut utiliser %c avec printf, ce sont des \b char
 */

enum kase {
	P = 'P', /*!<plaine*/
	F = 'F', /*!<forêt*/
	L = 'L', /*!<lac*/
	V = 'V', /*!<village*/
	R = 'R', /*!<ressource*/
	U = 'U', /*!<usine*/
	N = ' '  /*!<vide*/
};

typedef enum kase kase;

/**
 * \enum card
 * \brief Liste les différentes orientations possibles d'une tuile
 */

enum card{
	O_N,	/*!<Nord*/
	O_O,	/*!<Ouest*/
	O_S,	/*!<Sud*/
	O_E		/*!<Est*/
};

typedef enum card card;

/*  n =	[1,2,3]    =	[1,2,3,4,5,6]
 *	[4,5,6]
 *
 *  s =	[6,5,4]    =	[6,5,4,3,2,1]	
 *  	[3,2,1]
 *
 *  e =	[4,1] =	[4,1,5,2,6,3]
 *  	[5,2]
 *  	[6,3]
 *
 *  o =	[3,6] =	[3,6,2,5,1,4]
 *  	[2,5]
 *  	[1,4]
 */

/**
 * \struct tuile
 * \brief Représente une tuile (non posée)
 */

typedef struct {
	kase cases[6]; 		/*!<Les six cases de la tuile*/
	card orient; 		/*!<L'orientation de la tuile (utilise une énumération définie plus haut.)*/
	int identifiant;	/*!<L'identifiant fourni dans les fichiers importables*/
	int fromFile;	    /*!<(booléen) Si la tuile a été importée d'un fichier*/
} tuile;

/**
 * \typedef Tuile
 * \brief Pointeur vers une tuile
 */

typedef tuile* Tuile;

/**
 * \brief Affiche une tuile dans le terminal
 * \param t Une tuile
 */

void print_tuile(Tuile t);

/**
 * \brief Fait tourner la Tuile passée en paramètre d'un quart de tour dans le sens trigonomètrique
 * \param t Une Tuile
 */

void plus_pi_4(Tuile t);

/**
 * \brief Oriente la Tuile \b t dans la direction \b c demandée 
 * \param t Une Tuile
 * \param c Une direction
 */

void oriente(Tuile t, card c);

/**
 * \author Thomas Kowalski, Thibaud Milhaud
 * \brief Génère une Tuile aléatoire à l'aide la fonction rand
 */

Tuile rand_tuile(void);

/**
 * \author Thomas Kowalski
 * \brief Charge une tuile sérialisée depuis le fichier \b path.
 * \param \b path le chemin du fichier
 * \param \b tuilesImportees un pointeur vers un entier où le nombre de tuiles importées sera stocké
 * \return un tableau de Tuile contenant les tuiles chargées depuis le fichier
 */
Tuile* tuiles_from_file(char* path, int* tuilesImportees);

/**
 * \author Thomas Kowalski
 * \brief Sérialise la tuile \b t vers le fichier \b path.
 * \param \b path le fichier vers lequel exporter les tuiles
 * \param \b tuiles un tableau de tuiles à exporter
 * \param \b n le nombre de tuiles dans le tableau
 */
void exporter_tuiles(char* path, Tuile* tuiles, int n);

/**
 * \author Thomas Kowalski
 * \brief Libère la mémoire allouée utilisée par \b tab
 * \param \b le tableau à libérer
 * \param \b n la longueur du tableau
 */
void free_tableau(Tuile* tab, int n);

#endif
