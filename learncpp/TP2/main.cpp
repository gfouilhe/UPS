#include "Client.h"
#include <iostream>

int main() {

    Client cli1("Guilhem",Date(9,17,2000),Compte(0));
    Compte acc1 = cli1.getAcc();
    std::cout << "Compte de cli1 :" << std::endl;
    acc1.afficherSolde();

    Client cli2("Ludovic",Date(7,8,1999),Compte(500));
    Compte acc2 = cli2.getAcc();
    std::cout << "Compte de cli2 :" << std::endl;
    acc2.afficherSolde();

    acc2.virement(acc1,300.0);

    std::cout << "Compte de cli1 :" << std::endl;
    acc1.afficherSolde();

    std::cout << "Compte de cli2 :" << std::endl;
    acc2.afficherSolde();
    // const int MAX = 10 ;
    // Client banque1[MAX] ; // statique
    // Client * banque2 ; // dynamique
};