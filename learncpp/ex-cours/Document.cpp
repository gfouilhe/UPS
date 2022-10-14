#include <iostream>
#include "Document.h"
#include <string>


Document::Document(string titre, string auteur, int Taille) {

    this->titre = titre;
    this->auteur = auteur;
    this->taille = taille;
    this->nbElem = 0;
    this->mots = new string [taille];

}

Document::~Document() {
    delete[] this->mots;
}

Document::ajouter(string &mot) {
    if (nbElem < taille) {
        this->mots[nbElem++] = mot;
    }
}
