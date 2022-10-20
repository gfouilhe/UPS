#include "Banque.h"
#include <iostream>

using namespace std;

int main() {

    int MaxClient = 10;

    Banque banque1(MaxClient);
    banque1.afficher();
    Client cli1 = Client("Coline", Date(04,16,1998), Compte(413.2));
    Client cli2 = Client("Guilhem", Date(04,16,1998), Compte(413)); 
    banque1.ajouter(cli1);
    Banque banque2(banque1);
    banque2.ajouter(cli2);
    banque1.afficher();
    cout << banque1.appartient(cli1) << endl;
    cout << banque1.appartient(cli2) << endl;
    cout << banque2.appartient(cli1) << endl;
    delete & banque1;

};