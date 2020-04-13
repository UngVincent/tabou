#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cmath>
#include <iostream>


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
    static long aleatoireCeiling(long villes);
};

# endif
