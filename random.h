#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>
#include <time.h>

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
};

# endif
