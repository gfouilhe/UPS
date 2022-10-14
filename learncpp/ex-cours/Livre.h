#include <string>
#include <iostream>
#include "Document.h"

using namespace std;

class Livre : public Document {

protected:
    int nbPages;
    string maisonEdition;
public:
    Livre(string titre = '', string auteur = '', string maisonEdition = '', int taille = 0, int nbPages = 0);
    ~Livre();
}