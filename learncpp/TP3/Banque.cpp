#include "Banque.h"
#include <iostream>

using namespace std;

Banque::Banque(int max) {
    this->nbClients = 0;
    this->MaxClient = max;
    this->tab = new Client[MaxClient];
}

Banque::~Banque() {
    delete[] this->tab;
}

void Banque::afficher() {
    cout << "Nombre de Clients : " << nbClients << endl;
    cout << "Nombre Maximum de Clients : " << MaxClient << endl;
    cout << "Tableau : " << endl;
    for (int i=0; i<nbClients; i++){
		cout << tab[i];
        }
}