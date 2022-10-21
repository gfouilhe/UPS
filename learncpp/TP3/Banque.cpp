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

Banque::Banque(const Banque &b) {
    this->nbClients = b.nbClients;
    this->MaxClient = b.MaxClient;
    this->tab = new Client[MaxClient];
    for (int i=0 ; i<nbClients ; i++) {
        tab[i] = b.tab[i];
    }
}

void Banque::afficher() {
    cout << "Nombre de Clients : " << nbClients << endl;
    cout << "Nombre Maximum de Clients : " << MaxClient << endl;
    cout << "Clients : " << endl;
    for (int i=0; i<nbClients; i++){
		tab[i].afficherClient();
        cout << "===================" << endl;
        }
}

void Banque::ajouter(Client cli) { 
    if (nbClients < MaxClient) {
        this->tab[nbClients] = cli;
        nbClients++;
    }
    else
        cout << "Il n'y a plus de place pour un nouveau client." << endl;
}

void Banque::supprimer(Client cli) { 
    //supprime la première occurence de cli dans Banque
    bool shift = false;
    for (int i=0; i<nbClients; i++) {
        if (tab[i] == cli) {
            shift = true;
        }
        if (shift) {
            tab[i] = tab[i+1];
        }
    }
    nbClients--;
}

bool Banque::appartient(Client cli) {
    for (int i=0; i<nbClients; i++){
        if (tab[i] == cli) {return true;}
    }
    return false;
}

void Banque::transfertVers(Banque &b, Client cli) {
    if (this->appartient(cli)) {
    b.ajouter(cli);
    this->supprimer(cli);
    }
    else {
        cout << "Erreur : Le client : " << endl;
        cli.afficherClient(); 
        cout << endl << "n'appartient pas à la banque : " << endl;
        this->afficher();
    }
}