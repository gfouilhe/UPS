#include "Banque.h"
#include <iostream>

using namespace std;

int main() {

    int MaxClient = 10;

    Banque banque1(MaxClient);
    delete & banque1;

};