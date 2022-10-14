#include "Livre.h"
#include <iostream>
#include <string>

using namespace std;

Livre::Livre(string titre, string auteur, string maisonEdition, int taille, int nbPages) : Document(t,a,taille) {
    this->page = nbPages;
    this->maisonEdition = maisonEdition;
}
