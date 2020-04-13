#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cmath>
#include <iostream>
using namespace std;



class Random{
public:
    // initialise le generateur de variables aleatoire
    static void randomize()
    {
      srand ( time(NULL) );
    };

    // retourne une variable aleatoire entiere comprise entre 0 et (borne-1)
    static long aleatoire(long borne)
    {
      return(rand()%borne);
    };


    // retourne une variable aleatoire entiere comprise entre 
    // ceiling(alpha*0,5*sqrt(n)) et ceiling(alpha*1,5*sqrt(n))
    static long aleatoireCeiling(long villes, long alpha) {
        cout << "appel d'aleatoireCeiling" << endl;
        long lb = ceil(alpha*0.5*sqrt(villes));      // lower bound
        long hb = ceil(alpha*1.5*sqrt(villes));      // higher bound
        return(rand() % hb + lb);   //  in the range lb to hb (becarefull, greater nb first) 
    };
};

# endif
