/**
 * \file test.h
 * Ce fichier contient les différents tests de recouvrement utiles pour
 * vérifier le respect des règles lorsque l'on pose une tuile
 */


#if !defined YTEST_H
#define YTEST_H

#include "tuile.h"
#include "grille.h"

/**
 * \brief Vérifie que la Tuile que l'on veut poser en (\b i, \b j) est bien contenue dans la grille
 * \param t la Tuile
 * \param i l'ascisse du coin haut gauche de la tuile
 * \param j l'ordonnée de ce coin
 * \param g la Grille
 * \return 1 si la condition est vérifiée 0 sinon
 */

int is_ingrid(Tuile t, int i, int j, Grille g);

/**
 * \brief Vérifie que la Tuile recouvre au moins une case lorsqu'on la pose en (\b i, \b j)
 * \param t la Tuile
 * \param i l'ascisse du coin haut gauche de la tuile
 * \param j l'ordonnée de ce coin
 * \param g la Grille
 * \return 1 si la condition est vérifiée 0 sinon
 */

int is_connected(Tuile t, int i, int j, Grille g);

/**
 * \brief Vérifie que la tuile ne recouvre pas de lac
 * \param t la Tuile
 * \param i l'ascisse du coin haut gauche de la tuile
 * \param j l'ordonnée de ce coin
 * \param g la Grille
 * \return 1 si la condition est vérifiée 0 sinon
 */

int dont_fill_lake(Tuile t, int i, int j, Grille g);

/**
 * \brief Vérifie que la tuile ne recouvre pas une autre tuile
 * \param t la Tuile
 * \param pi l'ascisse du coin haut gauche de la tuile
 * \param pj l'ordonnée de ce coin
 * \param g la Grille
 * \return 1 si la condition est vérifiée 0 sinon
 */
int verifier_non_couvrement_total(Tuile t, int pi, int pj, Grille g);

/**
 * \brief teste si toutes les conditions pour pouvoir insérer une tuile sont réunies
 * \param t la Tuile
 * \param i l'ascisse du coin haut gauche de la tuile
 * \param j l'ordonnée de ce coin
 * \param g la Grille
 * \return 1 si la condition est vérifiée 0 sinon
 */

int test_tout(Tuile t, int i, int j, Grille g);

#endif
