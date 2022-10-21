#include "Client.h"

class Banque {

    Client * tab ; // tableau dynamique de clients
    int MaxClient; // taille max de tab
    int nbClients; // indice du premier emplacement libre dans tab

public:

    Banque(int max);
    ~Banque();
    Banque(const Banque &b);
    void afficher();
    void ajouter(Client cli);
    void supprimer(Client cli);
    bool appartient(Client cli);
    void transfertVers(Banque &b, Client cli);

} ;