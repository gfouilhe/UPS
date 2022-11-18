#include "Banque.h"

using namespace std;

int main() {

    int MaxClient = 10;

    Banque banque1(MaxClient);
    // banque1.afficher();
    Client cli1 = Client("Coline", Date(04,16,1998), Compte(413.2));
    Client cli2 = Client("Guilhem", Date(04,16,1998), Compte(413)); 
    banque1.ajouter(cli1);
    Banque banque2(MaxClient);
    banque2.ajouter(cli2);
    banque1.transfertVers(banque2,cli1);
    // cout <<"Banque 1 :" << endl;
    // // banque1.afficher();
    // cout <<"Banque 2 :" << banque2 << endl;

    // banque2.afficher();

    // cout << banque1.appartient(cli1) << endl;
    // cout << banque1.appartient(cli2) << endl;
    // cout << banque2.appartient(cli1) << endl;
    banque2.~Banque();
    cout <<"Banque 2 :" << banque2 << endl;

};