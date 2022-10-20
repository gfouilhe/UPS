#include "Compte.h"
#include <iostream>

using namespace std;

Compte::Compte() {this->solde = 0;}
Compte::Compte(float s) {this->solde = s;}
void Compte::depot(float m) {this->solde += m;}
void Compte::retrait(float m) {this->solde -= m;}
void Compte::afficherSolde() {cout<<"Le Solde est : " << this->solde << endl;}
void Compte::virement(Compte &c, float m) {
    c.depot(m);
    this->retrait(m);
    }
float Compte::getSolde() {return this->solde;}
bool Compte::operator==(const Compte &c) {
        return this-> solde == c.solde; 
    }


