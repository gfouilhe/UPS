#include "Client.h"

class Banque {

    Client * tab ; // tableau dynamique de clients
    int MaxClient; // taille max de tab
    int nbClients; // indice du premier emplacement libre dans tab

public:

    Banque(int max);
    ~Banque();

} ;