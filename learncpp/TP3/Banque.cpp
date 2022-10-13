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