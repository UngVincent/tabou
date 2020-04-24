#include "rechercheTabou.h"

using namespace std;

// Initialisation des param�tres de la RechercheTabou
// et g�n�ration de la solution initiale.
// Initialisation  de la liste tabou
rechercheTabou::rechercheTabou(int nbiter, int dt, int nv, char *nom_fichier)
{
  nbiterations = nbiter;
  iter_courante = 0;
  duree_tabou = dt;
  taille_solution = nv;
  alpha = dt;
  constant_FTD = 10;

  constuction_distance(taille_solution, nom_fichier);
  courant = new solution(nv);
  courant->evaluer(les_distances);

  list_tabou = new int *[nv];
  for (int i = 0; i < nv; i++)
  {
    dt_it_moyen[i] = 0;           // intitialisation pour compter les durees tabou moyennes
    dt_val_moyen[i] = 0;
    list_tabou[i] = new int[nv];
    for (int j = 0; j < nv; j++)
      list_tabou[i][j] = -1;
  }

  // it_villes_parcouru = new int *[nv];
  // for (int i = 0; i < duree_tabou; i++)
  // {
  //   it_villes_parcouru[i] = new int[taille_solution];
  //   for (int j = 0; j < taille_solution; j++)
  //     it_villes_parcouru[i][j] = 0;
  // }
  for (int i = 0; i < 100; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      it_villes_parcouru[i][j] = 0;
    }
  }

  cout << "La solution initiale aleatoire est   : ";
  courant->afficher();

  list_tabou2 = new int *[duree_tabou];
  for (int i = 0; i < duree_tabou; i++)
  {
    list_tabou2[i] = new int[taille_solution];
    for (int j = 0; j < taille_solution; j++)
      list_tabou2[i][j] = -1;
  }
}

rechercheTabou::~rechercheTabou()
{
  delete courant;
  for (int i = 0; i < taille_solution; i++)
  {
    delete list_tabou[i];
    delete les_distances[i];
  }
  /*  for(int i=0; i<duree_tabou; i++) // COMMENT IF TABOU 2 NOT USED
  {
    cout<<5<<endl;
    delete list_tabou2[i];*/
  delete[] list_tabou;
  //  delete[] list_tabou2;
  delete[] les_distances;
}

void rechercheTabou::constuction_distance(int nv, char *nom_fichier)
{
  les_distances = new int *[nv];
  for (int i = 0; i < nv; i++)
    les_distances[i] = new int[nv];

  ifstream fichier;
  // Ouvre le fichier des distances entre villes
  fichier.open(nom_fichier, ifstream::in);
  if (!fichier.is_open())
  {
    cerr << "Fichier [" << nom_fichier << "] invalide." << endl;
    exit(-1);
  }

  for (int i = 0; i < nv; i++)
  {
    for (int j = i + 1; j < nv; j++)
    {
      fichier >> les_distances[i][j];
      les_distances[j][i] = les_distances[i][j];
    }
  }

  for (int i = 0; i < nv; i++)
    les_distances[i][i] = -10;

  fichier.close();
}

bool rechercheTabou::nonTabou(int i, int j)
{
  if (list_tabou[i][j] < iter_courante)
    return true;
  else
    return false;
}

// non utilise
bool rechercheTabou::nonTabou2(solution *sol)
{
  for (int i = 0; i < duree_tabou; i++)
  {
    for (int j = 1; j < taille_solution; j++)
    {
      if (list_tabou2[i][j] != sol->ville[j])
        j = taille_solution;
      else if (j == taille_solution - 1)
        return false;
    }
  }
  for (int i = 0; i < duree_tabou; i++)
  {
    for (int j = 1; j < taille_solution; j++)
    {
      if (list_tabou2[i][j] != sol->ville[taille_solution - j])
        j = taille_solution;
      else if (j == taille_solution - 1)
        return false;
    }
  }
  return true;
}

// non utilise
void rechercheTabou::mise_a_jour_liste_tabou_2(solution *sol, int &position)
{
  if (duree_tabou != 0)
  {
    for (int j = 0; j < taille_solution; j++)
      list_tabou2[position][j] = sol->ville[j];
    position++;
    if (position == duree_tabou)
      position = 0;
  }
}

void rechercheTabou::voisinage_2_opt(int &best_i, int &best_j)
{
  int best_vois;
  bool tous_tabou = true;
  best_vois = 100000;
  // on selectionne une premiere ville pour le mouvement
  for (int i = 0; i < taille_solution; i++)
  {
    // on selectionne une seconde ville pour le mouvement
    for (int j = i + 1; j < taille_solution; j++)
    {
      if (((i != 0) || (j != taille_solution - 1)) && ((i != 0) || (j != taille_solution - 2)))
      {
        // on transforme la solution courante vers le voisin
        //    grace au mouvement definit par le couple de ville
        courant->inversion_sequence_villes(i, j);
        // on estime ce voisin
        courant->evaluer(les_distances);
        // si ce mouvement est non tabou et
        // si ce voisin a la meilleure fitness
        // alors ce voisin devient le meilleur voisin non tabou
        if (nonTabou(i, j) && courant->fitness < best_vois)
        //if(nonTabou2(courant) && courant->fitness<best_vois)
        {
          best_vois = courant->fitness;
          best_i = i;
          best_j = j;
          tous_tabou = false;
        }
        // on re-transforme ce voisin en la solution courante
        courant->inversion_sequence_villes(i, j);
        // on re-evalue la solution courante
        courant->evaluer(les_distances);
      }
    }
  }
}

//proc�dure principale de la recherche
solution *rechercheTabou::optimiser()
{
  bool first = true; // indique si c'est la premiere fois
  //         que l'on est dans un mimium local
  bool descente = false;      // indique si la solution courzntz corresponds � une descente
  int ameliore_solution = -1; // indique l'iteration o� l'on a am�lior� la solution
  int f_avant, f_apres;       // valeurs de la fitness avant et apr�s une it�ration

  // La meilleure solution trouv�e (= plus petit minium trouv�) � conserver
  solution *best_solution = new solution(taille_solution);

  int best_i = 0; // Le couple (best_i, best_j) repr�sente le meilleur mouvement non tabou
  int best_j = 0;
  int best_eval = courant->fitness;
  f_avant = 10000000;
  int nb_min_locaux = 0; // Permet de compter le nb de min locaux

  // Tant que le nombre d'it�rations limite n'est pas atteint
  for (iter_courante = 0; iter_courante < nbiterations; iter_courante++)
  {
    duree_tabou = constant_FTD + ceil(alpha * it_villes_parcouru[best_i][best_j] / taille_solution);
    voisinage_2_opt(best_i, best_j); // La fonction 'voisinage_2_opt' retourne le meilleur
    //   mouvement non tabou; c'est le couple (best_i, best_j)
    courant->inversion_sequence_villes(best_i, best_j);
    //  On d�place la solution courante gr�ce � ce mouvement

    dt_it_moyen[best_i] += 1;        // incremente les tableaux de valeur tabou moyens
    dt_it_moyen[best_j] += 1;
    dt_val_moyen[best_i] += (long) duree_tabou;
    dt_val_moyen[best_j] += (long) duree_tabou;        

    courant->ordonner();             // On r�ordonne la solution en commen�ant par 0
    courant->evaluer(les_distances); // On �value la nouvelle solution courante

    f_apres = courant->fitness; // valeur de la fitness apres le mouvement

    if (courant->fitness < best_eval)        // si on am�liore le plus petit minimum rencontr�
    {                                        // alors on l'enregistre dans 'best_solution'
      best_eval = courant->fitness;          // on mets � jour 'best_eval'
      best_solution->copier(courant);        // on enregistre la solution corante comme best_solution
      best_solution->evaluer(les_distances); // on �value la best solution
      ameliore_solution = iter_courante;     // on indique que l'am�lioration � eu lieu � cette it�ration
    }
    else // Si on n'est pas dans le plus petit minimum rencontr� mais dans un minimum local
    {
      // Crit�res de d�tection d'un minimum local. 2 cas:
      //  1. si la nouvelle solution est + mauvaise que l'ancienne
      //         et que on est en train d'effectuer une descente
      //  2. si la nouvelle solution est identique � l'ancienne
      //         et que c'est la premi�re fois que cela se produit
      if (((f_avant < f_apres) && (descente == true)) || ((f_avant == f_apres) && (first)))
      {

        nb_min_locaux += 1;
        /*  cout << "On est dans un minimum local a l'iteration "
		   << iter_courante-1 << " -> min = " << f_avant
		   << " km (le + petit min local deja trouve = "
		   << best_eval << " km)" << endl; */
        first = false;
      }

      if (f_avant <= f_apres) // la solution courente se d�grade
        descente = false;
      else
        descente = true; // la solution courante s'am�liore : descente

      if ((f_avant != f_apres) && (!first)) //
        first = true;
    }

    // mise � jour de la liste tabou
    list_tabou[best_i][best_j] = iter_courante + duree_tabou;
    // cout << duree_tabou << endl;
    // list_tabou[best_i][best_j] = iter_courante+(constant_FTD+alpha*list_tabou[best_i][best_j]/taille_solution);
    it_villes_parcouru[best_i][best_j] += 1;

    //mise_a_jour_liste_tabou_2(courant, position);
    f_avant = f_apres;

    if (iter_courante % 10000 == 0)
    {
      cout << "iteration courante : " << iter_courante << endl;
    }

    // output: index of iteration and the optimal solution so far en C
    //printf("%d\t%d\t%d\n", iter_courante, courant->fitness, best_eval);
    
  }

  ofstream myfile;
  myfile.open("data.txt", std::ios_base::app);
  myfile << best_eval << ";" << ameliore_solution << ";" << nb_min_locaux << ";" << nbiterations << ";" << alpha << ";" << taille_solution << endl; // best result; iter number; nb local minima; nb iterations; alpha; nb ville
  myfile.close();

  myfile.open("dt_data.txt", std::ios_base::app);
  for (int i = 0; i < 100; ++i){
      myfile << dt_val_moyen[i] << ";" << dt_it_moyen[i] << ";" << endl;
  }
  myfile.close();
  
  myfile.open("best_sol.txt", std::ios_base::app);
  for(int i=0;i<best_solution->taille;i++)
    myfile << best_solution->ville[i] << "-";
  myfile << "--> " << best_solution->fitness << " km" << endl;
    
  for(int i=0;i<best_solution->taille-1;i++)
    myfile << les_distances[best_solution->ville[i]][best_solution->ville[i+1]] << ";"; 
  myfile << les_distances[best_solution->ville[0]][best_solution->ville[best_solution->taille-1]] << endl;
  myfile.close();


  printf("BEST ITERATION = %d ; AND NB LOCAL MINIMA = %d\n", ameliore_solution, nb_min_locaux);
  return best_solution;
}
