#include "random.h"
using namespace std;


// retourne une variable aleatoire entiere comprise entre 
// ceiling(alpha*0,5*sqrt(n)) et ceiling(alpha*1,5*sqrt(n))
Random::aleatoireCeiling(long villes, long alpha) {
    cout << "appel d'aleatoireCeiling" << endl;
    long lb = ceil(alpha*0.5*sqrt(n))      // lower bound
    long hb = ceil(alpha*1.5*sqrt(n))      // higher bound
    return(rand() % hb + lb);   //  in the range lb to hb (becarefull, greater nb first) 
}