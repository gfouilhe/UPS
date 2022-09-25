#include "Compte.h"
#include <iostream>

using namespace std;

int main() {
    cout << "Compte 1" << endl;
    Compte acc;
    acc.afficherSolde();
    cout << "Ajout de 1200 euros sur le Compte 1" << endl;
    acc.depot(1200.0);
    acc.afficherSolde();
    cout << "Compte 2" << endl;
    Compte acc2(3000.0);
    acc2.afficherSolde();
    cout << "Transfert de 1000 euros depuis le compte 2 vers le Compte 1" << endl;
    acc2.virement(acc,1000.0);
    cout << "Compte 1" << endl;
    acc.afficherSolde();
    cout << "Compte 2" << endl;
    acc2.afficherSolde();

};