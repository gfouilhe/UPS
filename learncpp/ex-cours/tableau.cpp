#include "tableau.h"
using namespace std;
#include <iostream>

Tableau::Tableau(int taille): taille(taille), nbElem(0) {
    this->valeurs = new int[taille];
}

Tablau::Tableau(const Tableau &tab){
    this->taille = tab.taille;
    this->nbElem = tab.nbElem;
    this->valeurs = new int[taille];
    for (int i=0 ; i<nbElem ; i++) {
        valeurs[i] = tab.valeurs[i];
    }
}

Tableau::~Tableau(){
    delete[] this->valeurs;
}

void Tableau::ajoute(int v){
    if (nbElem < taille) {
        this->valeurs[nbElem] = v;
        nbElem++;
    }
}