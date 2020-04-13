#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>
#include <time.h>

class Random{
public:
    // initialise le g�n�rateur de variables al�atoire
    static void randomize()
    {
      srand ( time(NULL) );
    };

    // retourne une variable al�atoire enti�re comprise entre 0 et (borne-1)
    static long aleatoire(long borne)
    {
      return(rand()%borne);
    };

    // retourne une variable aleatoire entiere comprise entre 
    // ceiling(alpha*0,5*sqrt(n)) et ceiling(alpha*1,5*sqrt(n))mak
    static int aleatoireCeiling(int villes, int alpha) {
        int lb = 0; 
        int hb = 0;
        lb = int(ceil(alpha*0.5*sqrt(villes)));      // lower bound
        hb = int(ceil(alpha*1.5*sqrt(villes)));      // higher bound
        //int rom = ((hb-lb+1)) + lb;
        //printf("ok");
        //eturn rom;
        // cout << lb << " hb " << hb << endl;
        return(rand() % (hb-lb+1) + lb);   //  in the range lb to hb (becarefull, greater nb first) 
    };

};

# endif
