
#include "solution.h"

using namespace std;


//initialisation d'une solution : un cycle passant par toutes les villes
solution::solution(int nv)
{
	int a;
	bool recommence = true;
	taille          = nv;
	ville           = new int[taille];
    //  Arbitrairement, on choisit de toujours commencer par la ville 0
	ville[0]        = 0;
	for(int i=1; i<taille; i++)
	{
		recommence  = true;
		while(recommence)
		{
			recommence = false;
			// on tire aleatoirement la ville suivante
			a = Random::aleatoire(taille);
			// la ville ne doit pas etre deja dans le cycle
			// si tel est le cas on recommence
			for (int j=0; j<i; j++)
				if (a==ville[j])
					recommence = true;
		}
		ville[i] = a;
	}
    // on impose arbitrairement que ville[1] > ville[taille-1]
	ordonner();
}

solution::~solution()
{
	delete[] ville;
}

// evaluation d'une solution : c'est la somme des distances reliant les villes
void solution::evaluer(int **distance)
{
	fitness = 0;
	for(int i=0;i<taille-1;i++)
		fitness += distance[ville[i]][ville[i+1]];
	fitness+=distance[ville[0]][ville[taille-1]];
}

void solution::afficher()
{
	for(int i=0;i<taille;i++)
		cout << ville[i] << "-";
	cout << "--> " << fitness << " km" << endl;
}

// on impose arbitrairement que la 2ieme ville visitee (ville[1])
//   ait un n° plus petit que la derniere ville visitee (ville[taille-1])
//   i.e. : ville[1] > ville[taille-1]
void solution::ordonner()
{
	int inter, k;
	// Place la ville "0" en tete de la solution (ville[0])
	if (ville[0] != 0)
	{
		int position_0 = 0;
		int * ville_c  = new int[taille];
		for (int i=0; i<taille; i++)
		{
			ville_c[i] = ville[i];
			if (ville[i]==0)
				position_0 = i;
		}
		k = 0;
		for (int i=position_0; i<taille; i++)
		{
			ville[k] = ville_c[i];
			k++;
		}
		for (int i=0; i<position_0; i++)
		{
			ville[k] = ville_c[i];
			k++;
		}
		delete[] ville_c;
	}

	// Le numero de la 2eme ville doit etre plus petit que celui de la derniere ville
	if (ville[1] > ville[taille-1])
	{
		for(int k=1; k<=1+(taille-2)/2; k++)
		{
			inter = ville[k];
			ville[k] = ville[taille-k];
			ville[taille-k] = inter;
		}
	}
}

// on echange 2 villes dans la solution
void solution::echange_2_villes(int ville1, int ville2)
{
	int inter    = ville[ville1];
	ville[ville1] = ville[ville2];
	ville[ville2] = inter;
}

void solution::deplacement_1_ville(int ville1, int ville2)
{
	int inter = ville[ville1];

	// on transforme la solution courante vers le voisin
	//    grace au mouvement definit par le couple de ville
	if (ville1 < ville2)
	{
	    for (int k=ville1; k<ville2; k++)
            ville[k] = ville[k+1];
	}
	else
	{
	    for (int k=ville1; k>ville2; k--)
            ville[k] = ville[k-1];
	}
    ville[ville2] = inter;
}


// on inverse toute une sequence de villes dans la solution
void solution::inversion_sequence_villes(int ville1, int ville2)
{
	for(int k=ville1; k<=ville1+(ville2-ville1)/2; k++)
	    echange_2_villes(k, ville2+ville1-k);
}

bool solution::identique(solution* chro)
{
	for(int i=1; i<taille; i++)
		if (chro->ville [i] != this->ville[i])
			return false;
	return true;
}

// copie les villes d'une solution. la fitness n'est reprise
void solution::copier(solution* source)
{
	for(int i=0; i<taille; i++)
		ville[i] = source->ville[i];
}

