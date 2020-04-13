
#ifndef _RT_H
#define _RT_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include "random.h"
#include "solution.h"
#include <vector>
#include <cmath>

// cette classe definie les parametres d'une execution
// de la recherche tabou ainsi que la procedure principale de recherche
class rechercheTabou{
public:
	// ATTRIBUTS
	int iter_courante;
	int nbiterations;              // nombre d'iterations apres quoi la recherche est arretee
	int duree_tabou;               // duree tabou en nombre d'iterations
	int taille_solution;           // nombre de villes dans le solution
	solution *courant;             // solution courante geree par la RechercheTabou
	int **list_tabou2;             // liste des solutions tabou
	int **list_tabou;              // liste des durees tabou associees a chaque couple de villes
	int **les_distances;           // matrice des distances entre les villes	                                                 // le voisinage est au sens voisinage 2-opt

	// CONSTRUCTEURS
	rechercheTabou(int nbiter, int dt, int nv, char* nom_fichier);  // construction de la rechercheTabou
	~rechercheTabou();

	// METHODES
	bool nonTabou(int i,int j);                      // le couple (ville i, ville j) est tabou : oui ou non
	bool nonTabou2(solution* sol);                   // la solution 'sol' est tabou : oui ou non
	void voisinage_2_opt(int& best_i, int &best_j);  // donne le meilleur voisinage non tabou
	solution* optimiser();                           // lancement de la rechercheTabou
	void constuction_distance(int nv, char* nom_fichier);
	void mise_a_jour_liste_tabou_2(solution* sol, int& position);
	                                                 // ajout de la solution 'sol' a la position 'position'
	                                                 //  de la liste tabou
};

# endif
